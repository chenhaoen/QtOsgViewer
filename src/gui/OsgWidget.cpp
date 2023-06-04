#include "OsgWidget.h"
#include "PickHandler.h"

#include <osg/Camera>

#include <osg/DisplaySettings>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>

#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>

#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>

#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>

#include <cassert>

#include <stdexcept>
#include <vector>

#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>

namespace
{

	QRect makeRectangle(const QPoint& first, const QPoint& second)
	{
		// Relative to the first point, the second point may be in either one of the
		// four quadrants of an Euclidean coordinate system.
		//
		// We enumerate them in counter-clockwise order, starting from the lower-right
		// quadrant that corresponds to the default case:
		//
		//            |
		//       (3)  |  (4)
		//            |
		//     -------|-------
		//            |
		//       (2)  |  (1)
		//            |

		if (second.x() >= first.x() && second.y() >= first.y())
			return QRect(first, second);
		else if (second.x() < first.x() && second.y() >= first.y())
			return QRect(QPoint(second.x(), first.y()), QPoint(first.x(), second.y()));
		else if (second.x() < first.x() && second.y() < first.y())
			return QRect(second, first);
		else if (second.x() >= first.x() && second.y() < first.y())
			return QRect(QPoint(first.x(), second.y()), QPoint(second.x(), first.y()));

		// Should never reach that point...
		return QRect();
	}

}

namespace osgWidget
{
	void Viewer::setupThreading()
	{
		if (_threadingModel == SingleThreaded)
		{
			if (_threadsRunning)
				stopThreading();
		}
		else
		{
			if (!_threadsRunning)
				startThreading();
		}
	}
}

OsgWidget::OsgWidget(QWidget* parent,
	Qt::WindowFlags f)
	: QOpenGLWidget(parent, f)
	, graphicsWindow_(new osgViewer::GraphicsWindowEmbedded(this->x(),
		this->y(),
		this->width(),
		this->height()))
	, viewer_(new osgWidget::Viewer)
	, selectionActive_(false)
	, selectionFinished_(true)
	,m_root(new osg::Group)
{
	float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());
	auto pixelRatio = this->devicePixelRatio();

	osg::Camera* camera = new osg::Camera;
	camera->setViewport(0, 0, this->width() * pixelRatio, this->height() * pixelRatio);
	camera->setClearColor(osg::Vec4(105/255.0, 105 / 255.0, 105 / 255.0, 1.f));
	//camera->setProjectionMatrixAsPerspective(100, 100, 1.f, 1000.f);
	camera->setGraphicsContext(graphicsWindow_);

	osgViewer::View* view = new osgViewer::View;
	view->setCamera(camera);
	view->setSceneData(m_root);
	view->addEventHandler(new osgViewer::StatsHandler);
	view->addEventHandler(new PickHandler(this->devicePixelRatio()));

	osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator;
	manipulator->setAllowThrow(false);

	view->setCameraManipulator(manipulator);

	viewer_->addView(view);
	viewer_->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
	viewer_->realize();

	// This ensures that the widget will receive keyboard events. This focus
	// policy is not set by default. The default, Qt::NoFocus, will result in
	// keyboard events that are ignored.
	this->setFocusPolicy(Qt::StrongFocus);
	this->setMinimumSize(100, 100);

	// Ensures that the widget receives mouse move events even though no
	// mouse button has been pressed. We require this in order to let the
	// graphics window switch viewports properly.
	this->setMouseTracking(true);

	//view->addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

	// add the thread model handler
	view->addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	view->addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	view->addEventHandler(new osgViewer::StatsHandler);

	// add the help handler
	//view->addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

	// add the record camera path handler
	view->addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	view->addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	view->addEventHandler(new osgViewer::ScreenCaptureHandler);
}

void OsgWidget::insertNode(const QString& name, osg::ref_ptr<osg::Node> node)
{
	m_nodes.emplace(name, node);

	resetRoot();
}

void OsgWidget::eraseNode(const QString& name)
{
	m_nodes.erase(name);

	resetRoot();
}

void OsgWidget::clearNode()
{
	m_nodes.clear();

	resetRoot();
}

void OsgWidget::paintEvent(QPaintEvent* /* paintEvent */)
{
	this->makeCurrent();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	this->paintGL();

	if (selectionActive_ && !selectionFinished_)
	{
		painter.setPen(Qt::black);
		painter.setBrush(Qt::transparent);
		painter.drawRect(makeRectangle(selectionStart_, selectionEnd_));
	}

	painter.end();

	this->doneCurrent();
}

void OsgWidget::paintGL()
{
	viewer_->frame();
}

void OsgWidget::resizeGL(int width, int height)
{
	this->getEventQueue()->windowResize(this->x(), this->y(), width, height);
	graphicsWindow_->resized(this->x(), this->y(), width, height);

	this->onResize(width, height);
}

void OsgWidget::keyPressEvent(QKeyEvent* event)
{
	QString keyString = event->text();
	const char* keyData = keyString.toLocal8Bit().data();

	if (event->key() == Qt::Key_S)
	{
		selectionActive_ = !selectionActive_;

		// Further processing is required for the statistics handler here, so we do
		// not return right away.
	}
	else if (event->key() == Qt::Key_H)
	{
		this->onHome();
		return;
	}

	this->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void OsgWidget::keyReleaseEvent(QKeyEvent* event)
{
	QString keyString = event->text();
	const char* keyData = keyString.toLocal8Bit().data();

	this->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void OsgWidget::mouseMoveEvent(QMouseEvent* event)
{
	// Note that we have to check the buttons mask in order to see whether the
	// left button has been pressed. A call to `button()` will only result in
	// `Qt::NoButton` for mouse move events.
	if (selectionActive_ && event->buttons() & Qt::LeftButton)
	{
		selectionEnd_ = event->pos();

		// Ensures that new paint events are created while the user moves the
		// mouse.
		this->update();
	}
	else
	{
		auto pixelRatio = this->devicePixelRatio();

		this->getEventQueue()->mouseMotion(static_cast<float>(event->x() * pixelRatio),
			static_cast<float>(event->y() * pixelRatio));
	}
}

void OsgWidget::mousePressEvent(QMouseEvent* event)
{
	// Selection processing
	if (selectionActive_ && event->button() == Qt::LeftButton)
	{
		selectionStart_ = event->pos();
		selectionEnd_ = selectionStart_; // Deletes the old selection
		selectionFinished_ = false;           // As long as this is set, the rectangle will be drawn
	}

	// Normal processing
	else
	{
		// 1 = left mouse button
		// 2 = middle mouse button
		// 3 = right mouse button

		unsigned int button = 0;

		switch (event->button())
		{
		case Qt::LeftButton:
			button = 1;
			break;

		case Qt::MiddleButton:
			button = 2;
			break;

		case Qt::RightButton:
			button = 3;
			break;

		default:
			break;
		}

		auto pixelRatio = this->devicePixelRatio();

		this->getEventQueue()->mouseButtonPress(static_cast<float>(event->x() * pixelRatio),
			static_cast<float>(event->y() * pixelRatio),
			button);
	}
}

void OsgWidget::mouseReleaseEvent(QMouseEvent* event)
{
	// Selection processing: Store end position and obtain selected objects
	// through polytope intersection.
	if (selectionActive_ && event->button() == Qt::LeftButton)
	{
		selectionEnd_ = event->pos();
		selectionFinished_ = true; // Will force the painter to stop drawing the
		// selection rectangle

		this->processSelection();
	}

	// Normal processing
	else
	{
		// 1 = left mouse button
		// 2 = middle mouse button
		// 3 = right mouse button

		unsigned int button = 0;

		switch (event->button())
		{
		case Qt::LeftButton:
			button = 1;
			break;

		case Qt::MiddleButton:
			button = 2;
			break;

		case Qt::RightButton:
			button = 3;
			break;

		default:
			break;
		}

		auto pixelRatio = this->devicePixelRatio();

		this->getEventQueue()->mouseButtonRelease(static_cast<float>(pixelRatio * event->x()),
			static_cast<float>(pixelRatio * event->y()),
			button);
	}
}

void OsgWidget::wheelEvent(QWheelEvent* event)
{
	// Ignore wheel events as long as the selection is active.
	if (selectionActive_)
		return;

	event->accept();
	const QPoint angleDelta = event->angleDelta();

	osgGA::GUIEventAdapter::ScrollingMotion motion = angleDelta.y() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP
		: osgGA::GUIEventAdapter::SCROLL_DOWN;

	this->getEventQueue()->mouseScroll(motion);
}

bool OsgWidget::event(QEvent* event)
{
	bool handled = QOpenGLWidget::event(event);

	// This ensures that the OSG widget is always going to be repainted after the
	// user performed some interaction. Doing this in the event handler ensures
	// that we don't forget about some event and prevents duplicate code.
	switch (event->type())
	{
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
	case QEvent::MouseButtonDblClick:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseMove:
	case QEvent::Wheel:
		this->update();
		break;

	default:
		break;
	}

	return handled;
}

void OsgWidget::onHome()
{
	osgViewer::ViewerBase::Views views;
	viewer_->getViews(views);

	for (std::size_t i = 0; i < views.size(); i++)
	{
		osgViewer::View* view = views.at(i);
		view->home();
	}
}

void OsgWidget::onResize(int width, int height)
{
	std::vector<osg::Camera*> cameras;
	viewer_->getCameras(cameras);

	auto pixelRatio = this->devicePixelRatio();

	cameras[0]->setViewport(0, 0, width  * pixelRatio, height * pixelRatio);
}

osgGA::EventQueue* OsgWidget::getEventQueue() const
{
	osgGA::EventQueue* eventQueue = graphicsWindow_->getEventQueue();

	if (eventQueue)
		return eventQueue;
	else
		throw std::runtime_error("Unable to obtain valid event queue");
}

void OsgWidget::processSelection()
{

	QRect selectionRectangle = makeRectangle(selectionStart_, selectionEnd_);
	auto widgetHeight = this->height();
	auto pixelRatio = this->devicePixelRatio();

	double xMin = selectionRectangle.left();
	double xMax = selectionRectangle.right();
	double yMin = widgetHeight - selectionRectangle.bottom();
	double yMax = widgetHeight - selectionRectangle.top();

	xMin *= pixelRatio;
	yMin *= pixelRatio;
	xMax *= pixelRatio;
	yMax *= pixelRatio;

	osgUtil::PolytopeIntersector* polytopeIntersector
		= new osgUtil::PolytopeIntersector(osgUtil::PolytopeIntersector::WINDOW,
			xMin, yMin,
			xMax, yMax);

	// This limits the amount of intersections that are reported by the
	// polytope intersector. Using this setting, a single drawable will
	// appear at most once while calculating intersections. This is the
	// preferred and expected behaviour.
	polytopeIntersector->setIntersectionLimit(osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE);

	osgUtil::IntersectionVisitor iv(polytopeIntersector);

	for (unsigned int viewIndex = 0; viewIndex < viewer_->getNumViews(); viewIndex++)
	{
		qDebug() << "View index:" << viewIndex;

		osgViewer::View* view = viewer_->getView(viewIndex);

		if (!view)
			throw std::runtime_error("Unable to obtain valid view for selection processing");

		osg::Camera* camera = view->getCamera();

		if (!camera)
			throw std::runtime_error("Unable to obtain valid camera for selection processing");

		camera->accept(iv);

		if (!polytopeIntersector->containsIntersections())
			continue;

		auto intersections = polytopeIntersector->getIntersections();

		for (auto&& intersection : intersections)
			qDebug() << "Selected a drawable:" << QString::fromStdString(intersection.drawable->getName());
	}
}

void OsgWidget::resetRoot()
{
	m_root->removeChildren(0, m_root->getNumChildren() - 1);

	osg::ref_ptr<osg::Node> node;
	for (const auto& kv : m_nodes)
	{
		node = kv.second;
		m_root->addChild(node);
	}
}
