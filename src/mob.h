#ifndef MOB_H
#define MOB_H

#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>

namespace godot
{
    class Mob : public godot::RigidBody2D
    {
        GDCLASS(Mob, RigidBody2D);

    private:
        AnimatedSprite2D *m_animatedSprite2D;
        Array mob_types;
        
    protected:
        static void _bind_methods();

    public:
        void _ready() override;
        void onVisibleOnScreenNotifier2DScreenExited();
    };
}

#endif