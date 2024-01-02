#include "camera.h"
#include <iostream>

void camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front; 
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)); 
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
};

glm::mat4 camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
    //return glm::lookAt(Position, glm::vec3(0.0f,0.0f,0.0f), Up);
};

void camera::ProcessKeyboard(Camera_Movement direction, float delta_time)
{
    float velocity = MovementSpeed * delta_time;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += Up * velocity;
    if (direction == DOWN)
        Position -= Up * velocity;

};

void camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity * Zoom / 50;
    yoffset *= MouseSensitivity * Zoom / 50;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
};

void camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
};