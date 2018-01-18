#include "qmapboxgl_map_observer_p.hpp"

#include "qmapboxgl_p.hpp"

QMapboxGLMapObserverPrivate::QMapboxGLMapObserverPrivate(QMapboxGLPrivate *d)
    : d_ptr(d)
{
}

QMapboxGLMapObserverPrivate::~QMapboxGLMapObserverPrivate()
{
}

void QMapboxGLMapObserverPrivate::onCameraWillChange(mbgl::MapObserver::CameraChangeMode mode)
{
    if (mode == mbgl::MapObserver::CameraChangeMode::Immediate) {
        emit mapChanged(QMapboxGL::MapChangeRegionWillChange);
    } else {
        emit mapChanged(QMapboxGL::MapChangeRegionWillChangeAnimated);
    }
}

void QMapboxGLMapObserverPrivate::onCameraIsChanging()
{
    emit mapChanged(QMapboxGL::MapChangeRegionIsChanging);
}

void QMapboxGLMapObserverPrivate::onCameraDidChange(mbgl::MapObserver::CameraChangeMode mode)
{
    if (mode == mbgl::MapObserver::CameraChangeMode::Immediate) {
        emit mapChanged(QMapboxGL::MapChangeRegionDidChange);
    } else {
        emit mapChanged(QMapboxGL::MapChangeRegionDidChangeAnimated);
    }
}

void QMapboxGLMapObserverPrivate::onWillStartLoadingMap()
{
    emit mapChanged(QMapboxGL::MapChangeWillStartLoadingMap);
}

void QMapboxGLMapObserverPrivate::onDidFinishLoadingMap()
{
    emit mapChanged(QMapboxGL::MapChangeDidFinishLoadingMap);
}

void QMapboxGLMapObserverPrivate::onDidFailLoadingMap(std::exception_ptr)
{
    emit mapChanged(QMapboxGL::MapChangeDidFailLoadingMap);
}

void QMapboxGLMapObserverPrivate::onWillStartRenderingFrame()
{
    emit mapChanged(QMapboxGL::MapChangeWillStartRenderingFrame);
}

void QMapboxGLMapObserverPrivate::onDidFinishRenderingFrame(mbgl::MapObserver::RenderMode mode)
{
    if (mode == mbgl::MapObserver::RenderMode::Partial) {
        emit mapChanged(QMapboxGL::MapChangeDidFinishRenderingFrame);
    } else {
        emit mapChanged(QMapboxGL::MapChangeDidFinishRenderingFrameFullyRendered);
    }
}

void QMapboxGLMapObserverPrivate::onWillStartRenderingMap()
{
    emit mapChanged(QMapboxGL::MapChangeWillStartLoadingMap);
}

void QMapboxGLMapObserverPrivate::onDidFinishRenderingMap(mbgl::MapObserver::RenderMode mode)
{
    if (mode == mbgl::MapObserver::RenderMode::Partial) {
        emit mapChanged(QMapboxGL::MapChangeDidFinishRenderingMap);
    } else {
        emit mapChanged(QMapboxGL::MapChangeDidFinishRenderingMapFullyRendered);
    }
}

void QMapboxGLMapObserverPrivate::onDidFinishLoadingStyle()
{
    emit mapChanged(QMapboxGL::MapChangeDidFinishLoadingStyle);
}

void QMapboxGLMapObserverPrivate::onSourceChanged(mbgl::style::Source&)
{
    std::string attribution;
    for (const auto& source : d_ptr->mapObj->getStyle().getSources()) {
        // Avoid duplicates by using the most complete attribution HTML snippet.
        if (source->getAttribution() && (attribution.size() < source->getAttribution()->size()))
            attribution = *source->getAttribution();
    }
    emit copyrightsChanged(QString::fromStdString(attribution));
    emit mapChanged(QMapboxGL::MapChangeSourceDidChange);
}
