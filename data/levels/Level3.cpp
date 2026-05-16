#include "Level3.h"

Level3::Level3(QObject* parent)
    : Level(
        ":/assets/sprites/levels/level3.png",
        ":/assets/sprites/levels/level3_collision_data.json",
        "",
        "level3",
        nullptr,
        std::make_unique<AnimatedSprite>(
            ":/assets/sprites/levels/level3_top.json",
            ":/assets/sprites/levels/level3_top.png"
        ),
        parent
    ),
    m_windowSprite(std::make_unique<AnimatedSprite>(
        ":/assets/sprites/objects/window.json",
        ":/assets/sprites/objects/window.png"
    )),
    m_windowLightSprite(std::make_unique<AnimatedSprite>(
        ":/assets/sprites/objects/window_light.json",
        ":/assets/sprites/objects/window_light.png"
    ))
{}
void Level3::update(int deltaTimeMs) {
    m_animationAccumulator += deltaTimeMs;
    if (m_animationAccumulator >= 250) {
        if (m_bottomSprite) m_bottomSprite->update(m_animationAccumulator);
        if (m_topSprite) m_topSprite->update(m_animationAccumulator);
        if (m_windowSprite) m_windowSprite->update(m_animationAccumulator);
        m_animationAccumulator = 0;
    }
}

void Level3::drawWindowLayer(QPainter& painter, const Camera& camera) {
    if (!m_windowSprite || !m_windowLightSprite) return;
    float scale = Constants::GAME_SCALE;
    float yOffset = verticalOffset();

    const std::vector<QPointF> points = {
        {130, 75}, {194, 75},
        {178, 818}, {519, 808},
        {485, 458}, {549, 458},
        {498, 110}, {562, 110},
        {738,110}
    };

    float windowWidth = 29 * scale;
    float windowHeight = 31 * scale;
    float lightWidth = 96 * scale;
    float lightHeight = 96 * scale;
    float lightOffsetX = -126;
    float lightOffsetY = -7;

    for (const QPointF& point : points) {
        float worldX = point.x() * scale;
        float worldY = point.y() * scale + yOffset;
        QRectF windowRect(
            camera.toScreenX(worldX),
            camera.toScreenY(worldY),
            windowWidth, windowHeight
        );
        m_windowSprite->draw(painter, windowRect, false);
        QRectF lightRect(
            camera.toScreenX(worldX + lightOffsetX),
            camera.toScreenY(worldY + lightOffsetY),
            lightWidth, lightHeight
        );
        m_windowLightSprite->draw(painter, lightRect, false);
    }
}