#include "mob.h"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/visible_on_screen_notifier2d.hpp>
#include <godot_cpp/classes/visible_on_screen_notifier2d.hpp>

using namespace godot;

void Mob::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onVisibleOnScreenNotifier2DScreenExited"), &Mob::onVisibleOnScreenNotifier2DScreenExited);
}

void Mob::_ready() {
    m_animatedSprite2D = get_node<AnimatedSprite2D>("AnimatedSprite2D");
    mob_types = Array(m_animatedSprite2D->get_sprite_frames()->get_animation_names());
    m_animatedSprite2D->get_animation() = mob_types.pick_random();
    get_node<VisibleOnScreenNotifier2D>("VisibleOnScreenNotifier2D")->connect("screen_exited", Callable(this, "onVisibleOnScreenNotifier2DScreenExited"), CONNECT_PERSIST);

}

void Mob::onVisibleOnScreenNotifier2DScreenExited()
{
    queue_free();
}