#ifndef OsgWidget_h__
#define OsgWidget_h__

#include <QPoint>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <osg/ref_ptr>

#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>

namespace osgWidget
{
	//! The subclass of osgViewer::CompositeViewer we use
	/*!
	 * This subclassing allows us to remove the annoying automatic
	 * setting of the CPU affinity to core 0 by osgViewer::ViewerBase,
	 * osgViewer::CompositeViewer's base class.
	 */
	class Viewer : public osgViewer::CompositeViewer
	{
	public:
		virtual void setupThreading();
	};
}

class OsgWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OsgWidget(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowType::Widget);

	void insertNode(const QString& name, osg::ref_ptr<osg::Node> node);
	void eraseNode(const QString& name);
	void clearNode();

protected:

	virtual void paintEvent(QPaintEvent* paintEvent);
	virtual void paintGL();
	virtual void resizeGL(int width, int height);

	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);

	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	virtual bool event(QEvent* event);

private:

	virtual void onHome();
	virtual void onResize(int width, int height);

	osgGA::EventQueue* getEventQueue() const;

	void processSelection();

	void resetRoot();
private:

	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow_;
	osg::ref_ptr<osgWidget::Viewer> viewer_;

	QPoint selectionStart_;
	QPoint selectionEnd_;

	bool selectionActive_;
	bool selectionFinished_;



	std::map<QString, osg::ref_ptr<osg::Node>> m_nodes;

	osg::ref_ptr<osg::Group> m_root;
};

#endif
