#include "player.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/marker2d.hpp>

using namespace godot;

void Player::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("hit"));
    ClassDB::bind_method(D_METHOD("onBodyEntered"), &Player::onBodyEntered);
    ClassDB::bind_method(D_METHOD("start"), &Player::start);
}

Player::Player()
{
    m_input = Input::get_singleton();
    m_nSpeed = 400.00;
}

Player::~Player()
{
    // Add your cleanup here.
}

void Player::_ready()
{
    m_screenSize = CanvasItem::get_viewport_rect().size;
    m_animatedSprite2d = get_node<AnimatedSprite2D>("AnimatedSprite2D");
    connect("body_entered", Callable(this, "onBodyEntered"), CONNECT_PERSIST);
    m_collisionShape2d = get_node<CollisionShape2D>("CollisionShape2D");
    hide();
}

void Player::_process(double delta)
{
    Vector2 velocity = Vector2(0, 0);
    m_position = get_position();
    if (m_input->is_action_pressed("ui_right"))
    {
        velocity.x += 1;
    }
    if (m_input->is_action_pressed("ui_left"))
    {
        velocity.x -= 1;
    }
    if (m_input->is_action_pressed("ui_down"))
    {
        velocity.y += 1;
    }
    if (m_input->is_action_pressed("ui_up"))
    {
        velocity.y -= 1;
    }

    if (velocity.length() > 0)
    {
        velocity = velocity.normalized() * m_nSpeed;
        m_animatedSprite2d->play();
        m_position += velocity * delta;
        set_position(m_position.clamp(Vector2(0, 0), m_screenSize));
    }
    else
    {
        m_animatedSprite2d->stop();
    }

    if (velocity.x != 0)
    {
        m_animatedSprite2d->get_animation() = StringName("walk");
        if (velocity.x > 0)
        {
            m_animatedSprite2d->set_flip_h(false);
        }
        else
        {
            m_animatedSprite2d->set_flip_h(true);
        }
    }
    else if (velocity.y != 0)
    {
        m_animatedSprite2d->get_animation() = StringName("up");
        if (velocity.y > 0)
        {
            m_animatedSprite2d->set_flip_v(true);
        }
        else
        {
            m_animatedSprite2d->set_flip_v(false);
        }
    }
    // 如果不移动的话自动反转y轴，看起来像是在地上
    else if (velocity.x == 0 && velocity.y == 0)
    {
        m_animatedSprite2d->set_flip_v(false);
    }
}

void Player::onBodyEntered(Node2D *body)
{
    hide();
    emit_signal("hit"); // Call the signal
    m_collisionShape2d->set_deferred("disabled", true);
}

void Player::start(Vector2 position)
{
    set_position(position);
    show();
    m_collisionShape2d->set_disabled(false);
}
