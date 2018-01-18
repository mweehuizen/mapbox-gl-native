#pragma once

#include "qmapboxgl.hpp"

#include <mbgl/renderer/renderer.hpp>
#include <mbgl/renderer/renderer_backend.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/util/shared_thread_pool.hpp>

#include <memory>
#include <mutex>

namespace mbgl {
class Renderer;
class UpdateParameters;
} // namespace mbgl

class QMapboxGLRendererBackendPrivate : public mbgl::RendererBackend
{
public:
    QMapboxGLRendererBackendPrivate(qreal pixelRatio, mbgl::DefaultFileSource &,
            mbgl::ThreadPool &, QMapboxGLSettings::GLContextMode);
    virtual ~QMapboxGLRendererBackendPrivate();

    // mbgl::RendererBackend implementation
    void updateAssumedState() final {}
    void bind() final {}
    mbgl::Size getFramebufferSize() const final { return mbgl::Size(800, 600); }
    mbgl::gl::ProcAddress getExtensionFunctionPointer(const char*) final;
    void activate() {}
    void deactivate() {}

    void update(std::shared_ptr<mbgl::UpdateParameters>);
    void render();

private:
    Q_DISABLE_COPY(QMapboxGLRendererBackendPrivate)

    mbgl::Renderer m_renderer;

    std::mutex m_updateMutex;
    std::shared_ptr<mbgl::UpdateParameters> m_updateParameters;
};
