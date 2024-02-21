#include "SD_PCH.h"

#include "Soda/_Main/Core.h"

#include "OrthoCameraController.h"

#include "Soda/Input/Input.h"
#include "Soda/Input/KeyboardCodes.h"
#include "Soda/Input/MouseCodes.h"


namespace Soda
{
    OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void OrthoCameraController::OnUpdate(float dt)
    {
        if (Input::IsKeyPressed(SD_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
        else if (Input::IsKeyPressed(SD_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * dt;

        if (Input::IsKeyPressed(SD_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * dt;
        else if (Input::IsKeyPressed(SD_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * dt;

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(SD_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * dt;
            if (Input::IsKeyPressed(SD_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * dt;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);
    }

    void OrthoCameraController::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrollEvent>(BIND_FN(OrthoCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_FN(OrthoCameraController::OnWindowResized));
    }

    bool OrthoCameraController::OnMouseScrolled(MouseScrollEvent& msEvent)
    {
        m_CameraTranslationSpeed = m_ZoomLevel;

        m_ZoomLevel -= msEvent.GetWheelOffsetY() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }

    bool OrthoCameraController::OnWindowResized(WindowResizeEvent& wrEvent)
    {
        m_AspectRatio = (float)wrEvent.GetWindowWidth() / (float)wrEvent.GetWindowHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }
}