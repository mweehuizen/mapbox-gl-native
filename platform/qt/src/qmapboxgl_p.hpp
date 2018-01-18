#pragma once

#include "qmapboxgl.hpp"
#include "qmapboxgl_map_observer_p.hpp"
#include "qmapboxgl_renderer_backend_p.hpp"

#include <mbgl/actor/actor.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/renderer/renderer_frontend.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/storage/resource_transform.hpp>
#include <mbgl/util/default_thread_pool.hpp>
#include <mbgl/util/geo.hpp>

#include <QObject>
#include <QSize>

#include <memory>

class QMapboxGLPrivate : public QObject, public mbgl::RendererFrontend
{
    Q_OBJECT

public:
    explicit QMapboxGLPrivate(QMapboxGL *, const QMapboxGLSettings &, const QSize &size, qreal pixelRatio);
    virtual ~QMapboxGLPrivate();

    // mbgl::RendererFrontend implementation.
    void reset() final {}
    void setObserver(mbgl::RendererObserver &) final {}
    void update(std::shared_ptr<mbgl::UpdateParameters>) final {}

    mbgl::EdgeInsets margins;
    QSize size { 0, 0 };
    QSize fbSize { 0, 0 };
    quint32 fbObject = 0;

    qreal pixelRatio;
    QMapboxGLSettings::GLContextMode mode;

    QMapboxGL *q_ptr { nullptr };

    std::shared_ptr<mbgl::DefaultFileSource> fileSourceObj;
    std::shared_ptr<mbgl::ThreadPool> threadPool;
    std::unique_ptr<mbgl::Map> mapObj;
    std::unique_ptr<QMapboxGLMapObserverPrivate> mapObserver;

    bool dirty { false };

public slots:
    void connectionEstablished();
    void render();

signals:
    void needsRendering();

private:
    Q_DISABLE_COPY(QMapboxGLPrivate)

    std::unique_ptr<mbgl::Actor<mbgl::ResourceTransform>> m_resourceTransform;
    std::unique_ptr<QMapboxGLRendererBackendPrivate> m_backend;
};
