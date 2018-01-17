#include "qmapboxgl_renderer_frontend_p.hpp"

#include <mbgl/actor/scheduler.hpp>
#include <mbgl/renderer/backend_scope.hpp>
#include <mbgl/renderer/renderer.hpp>

QMapboxGLRendererFrontend::QMapboxGLRendererFrontend(std::unique_ptr<mbgl::Renderer> renderer_, mbgl::RendererBackend& backend_)
    : renderer(std::move(renderer_))
    , backend(backend_) {
}

QMapboxGLRendererFrontend::~QMapboxGLRendererFrontend() = default;

void QMapboxGLRendererFrontend::reset() {
    if (renderer) {
        renderer.reset();
    }
}

void QMapboxGLRendererFrontend::update(std::shared_ptr<mbgl::UpdateParameters> updateParameters_) {
    std::lock_guard<std::mutex> lock(updateMutex);
    updateParameters = std::move(updateParameters_);
    emit updated();
}

void QMapboxGLRendererFrontend::setObserver(mbgl::RendererObserver& observer_) {
    if (!renderer) return;
    
    renderer->setObserver(&observer_);
}

void QMapboxGLRendererFrontend::render() {
    std::shared_ptr<mbgl::UpdateParameters> params;
    {
        // Lock on the parameters
        std::unique_lock<std::mutex> lock(updateMutex);
        if (!updateParameters) return;

        // Hold on to the update parameters during render
        params = updateParameters;
    }

    if (!renderer) return;

    // The OpenGL implementation automatically enables the OpenGL context for us.
    mbgl::BackendScope scope { backend, mbgl::BackendScope::ScopeType::Implicit };

    renderer->render(*params);
}
