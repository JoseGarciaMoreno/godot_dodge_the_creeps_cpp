#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>


namespace godot
{

    class Player : public Area2D
    {
        GDCLASS(Player, Area2D)

    private:
        Input *m_input;
        Size2 m_screenSize;
        double m_nSpeed;
        AnimatedSprite2D *m_animatedSprite2d;
        Vector2 m_position;
        CollisionShape2D *m_collisionShape2d;

    protected:
        static void _bind_methods();

    public:
        Player();
        ~Player();

        void _ready() override;
        void _process(double delta) override;
        void onBodyEntered(Node2D *body);// singal
        void start(Vector2 position);
    };

}

#endif