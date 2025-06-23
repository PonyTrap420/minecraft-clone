#include "player.h"
#include <stdlib.h>

static Player* playerInstance = NULL;

Player* player_get_instance(void) {
    if (!playerInstance) {
        playerInstance = malloc(sizeof(Player));

        Camera* cam = &playerInstance->camera;
        glm_vec3_copy((vec3){0.0f, 1.6f, 3.0f}, cam->cameraPos);
        glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, cam->cameraFront);
        glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, cam->cameraUp);
        cam->yaw = YAW;
        cam->pitch = PITCH;
        cam->fov = ZOOM;

        glm_vec3_zero(playerInstance->velocity);
        playerInstance->isFlying = false;
        playerInstance->onGround = false;
        playerInstance->jumpStrength = JUMP;
        playerInstance->height = HEIGHT;
        playerInstance->width = 0.6;
        playerInstance->moveSpeed = 7.5f;
    }
    return playerInstance;
}

bool player_collides_at(World* world, float x, float y, float z, float width, float height) {
    int minX = (int)floorf(x - width / 2.0f);
    int maxX = (int)floorf(x + width / 2.0f);
    int minY = (int)floorf(y);
    int maxY = (int)floorf(y + height);
    int minZ = (int)floorf(z - width / 2.0f);
    int maxZ = (int)floorf(z + width / 2.0f);

    for (int xi = minX; xi <= maxX; xi++) {
        for (int yi = minY; yi <= maxY; yi++) {
            for (int zi = minZ; zi <= maxZ; zi++) {
                if (world_get_block(world, xi, yi, zi) != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void player_update(Player* player, float deltaTime, World* world, GLFWwindow* window) {
    Camera* cam = &player->camera;

    double currentTime = glfwGetTime();

    bool forward  = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool left     = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool right    = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
                       glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    bool ctrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || 
                      glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

    static bool wasSneaking = false;
    float feetY = cam->cameraPos[1] - player->height;

    if (ctrlPressed && !wasSneaking) {
        float heightDifference = HEIGHT - SNEAK_HEIGHT;
        cam->cameraPos[1] -= heightDifference;
        player->height = SNEAK_HEIGHT;
        wasSneaking = true;
    } else if (!ctrlPressed && wasSneaking) {
        float heightDifference = HEIGHT - SNEAK_HEIGHT;
        cam->cameraPos[1] += heightDifference;
        player->height = HEIGHT;
        wasSneaking = false;
    }

    float currentSpeed = player->moveSpeed;
    
    if (ctrlPressed) {
        currentSpeed *= 0.3f;
    } else if (shiftPressed && forward && !backward && !left && !right) {
        currentSpeed *= 1.0f;
    } else {
        currentSpeed *= 0.5f;
    }
    
    float velocity = currentSpeed * deltaTime;

    vec3 frontFlat;
    glm_vec3_copy(cam->cameraFront, frontFlat);
    frontFlat[1] = 0.0f;
    glm_normalize(frontFlat);

    vec3 rightVec;
    glm_vec3_crossn(frontFlat, cam->cameraUp, rightVec);

    vec3 proposedPos;
    glm_vec3_copy(cam->cameraPos, proposedPos);

    vec3 moveVector = {0.0f, 0.0f, 0.0f};
    
    if (forward) {
        moveVector[0] += frontFlat[0];
        moveVector[2] += frontFlat[2];
    }
    if (backward) {
        moveVector[0] -= frontFlat[0];
        moveVector[2] -= frontFlat[2];
    }
    if (left) {
        moveVector[0] -= rightVec[0];
        moveVector[2] -= rightVec[2];
    }
    if (right) {
        moveVector[0] += rightVec[0];
        moveVector[2] += rightVec[2];
    }

    float moveLength = sqrtf(moveVector[0] * moveVector[0] + moveVector[2] * moveVector[2]);
    if (moveLength > 0.0f) {
        moveVector[0] /= moveLength;
        moveVector[2] /= moveLength;
    }

    proposedPos[0] += moveVector[0] * velocity;
    proposedPos[2] += moveVector[2] * velocity;

    if (ctrlPressed) {
        float checkY = feetY - 1.0f;
        
        bool hasBlockBelow = player_collides_at(world, proposedPos[0], checkY, proposedPos[2], player->width, 1.0f);
        
        if (!hasBlockBelow) {
            proposedPos[0] = cam->cameraPos[0];
            proposedPos[2] = cam->cameraPos[2];
        }
    }

    if (!player_collides_at(world, proposedPos[0], feetY, cam->cameraPos[2], player->width, player->height)) {
        cam->cameraPos[0] = proposedPos[0];
    }

    if (!player_collides_at(world, cam->cameraPos[0], feetY, proposedPos[2], player->width, player->height)) {
        cam->cameraPos[2] = proposedPos[2];
    }

    static bool spacePressedLastFrame = false;
    static double lastSpaceTime = 0.0;
    static double lastJumpTime = 0.0;
    static double coyoteTimeStart = 0.0;
    static bool wasOnGround = false;

    if (player->onGround) {
        coyoteTimeStart = currentTime;
        wasOnGround = true;
    } else if (wasOnGround && !player->onGround) {
        coyoteTimeStart = currentTime;
        wasOnGround = false;
    }

    bool inCoyoteTime = (currentTime - coyoteTimeStart) < 0.1f;
    bool canJump = player->onGround || inCoyoteTime;

    if (spacePressed && !spacePressedLastFrame) {
        if (currentTime - lastSpaceTime < 0.2) {
            player->isFlying = !player->isFlying;
            player->velocity[1] = 0.0f;
            lastSpaceTime = 0.0;
        } else {
            if (!player->isFlying && canJump && (currentTime - lastJumpTime) > 0.1) {
                player->velocity[1] = player->jumpStrength;
                player->onGround = false;
                lastJumpTime = currentTime;
                coyoteTimeStart = 0.0;
            }
            lastSpaceTime = currentTime;
        }
    }
    spacePressedLastFrame = spacePressed;

    if (!player->isFlying) {
        if (!player->onGround) {
            player->velocity[1] += GRAVITY * deltaTime;
        }

        float proposedY = cam->cameraPos[1] + player->velocity[1] * deltaTime;
        float proposedFeetY = proposedY - player->height;

        if (!player_collides_at(world, cam->cameraPos[0], proposedFeetY, cam->cameraPos[2], player->width, player->height)) {
            cam->cameraPos[1] = proposedY;
            player->onGround = false;
        } else {
            if (player->velocity[1] < 0) {
                player->onGround = true;
                float groundY = floorf(feetY);
                cam->cameraPos[1] = groundY + player->height + 0.001f;
            }
            player->velocity[1] = 0.0f;
        }

    } else {
        camera_process_input(cam, window);
        
        bool upPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        bool downPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
                          glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        
        float flySpeed = currentSpeed * 0.5f;
        
        if (upPressed) {
            cam->cameraPos[1] += flySpeed * deltaTime;
        }
        if (downPressed) {
            cam->cameraPos[1] -= flySpeed * deltaTime;
        }
    }

    bool isMoving = forward || backward || left || right;
    player->isMoving = isMoving;
    
    if (isMoving && player->onGround && !player->isFlying && !ctrlPressed) {
        player->cameraBobTime += deltaTime * 12.0f;
        
        vec3 camRight;
        glm_vec3_crossn(cam->cameraFront, cam->cameraUp, camRight);
        
        float horizontalBob = sinf(player->cameraBobTime) * 0.06f; 
        float verticalBob = sinf(player->cameraBobTime * 2.0f) * 0.04f;
        
        cam->bobbingOffset[0] = camRight[0] * horizontalBob;
        cam->bobbingOffset[2] = camRight[2] * horizontalBob;
        cam->bobbingOffset[1] = verticalBob;
        
    } else {
        glm_vec3_scale(cam->bobbingOffset, 0.95f, cam->bobbingOffset);
    }
}