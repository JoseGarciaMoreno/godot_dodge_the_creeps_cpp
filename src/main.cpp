#include "main.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void Main::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_mob_scene"), &Main::set_mob_scene);
    ClassDB::bind_method(D_METHOD("get_mob_scene"), &Main::get_mob_scene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "m_mob_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_mob_scene", "get_mob_scene");
    ClassDB::bind_method(D_METHOD("onMobTimerTimeout"), &Main::onMobTimerTimeout);
    ClassDB::bind_method(D_METHOD("onScroeTimerTimeout"), &Main::onScroeTimerTimeout);
    ClassDB::bind_method(D_METHOD("onStartTimerTimeout"), &Main::onStartTimerTimeout);
    ClassDB::bind_method(D_METHOD("newGame"), &Main::newGame);
    ClassDB::bind_method(D_METHOD("gameOver"), &Main::gameOver);
}

Main::Main()
{
    m_mob = ResourceLoader::get_singleton()->load("res://mob/mob.tscn");
}

Main::~Main()
{
}

void Main::_ready()
{
    set_mob_scene(m_mob);
    m_scoreTimer = get_node<Timer>("ScoreTimer");
    m_mobTimer = get_node<Timer>("MobTimer");
    m_player = get_node<Player>("Player");
    m_startTimer = get_node<Timer>("StartTimer");
    m_startPosition = get_node<Marker2D>("StartPosition");
    m_scoreTimer->connect("timeout", Callable(this, "onScroeTimerTimeout"), CONNECT_PERSIST);
    m_startTimer->connect("timeout", Callable(this, "onStartTimerTimeout"), CONNECT_PERSIST);
    m_mobTimer->connect("timeout", Callable(this, "onMobTimerTimeout"), CONNECT_PERSIST);
    m_mobPath = get_node<Path2D>("MobPath");
    m_mobSpawnLocation = cast_to<PathFollow2D>(get_node_or_null("MobPath/MobSpawnLocation"));
    m_hud = get_node<Hud>("Hud");
    m_hud->connect("start_game", Callable(this, "newGame"), CONNECT_PERSIST);
    m_player->connect("hit", Callable(this, "gameOver"), CONNECT_PERSIST);
    m_music = get_node<AudioStreamPlayer2D>("Music");
    m_deathSound = get_node<AudioStreamPlayer2D>("DeathSound");
}

void Main::set_mob_scene(const Ref<PackedScene> &p_scene)
{
    m_mobScene = p_scene;
}

Ref<PackedScene> Main::get_mob_scene() const
{
    return m_mobScene;
}

void Main::gameOver()
{
    m_music->stop();
    m_deathSound->play();
    m_scoreTimer->stop();
    m_mobTimer->stop();
    m_hud->showGameOver();
}

void Main::newGame()
{
    m_music->play();
    get_tree()->call_group("mobs", "queue_free");
    m_nScore = 0;
    m_player->start(m_startPosition->get_position());
    m_startTimer->start();
    m_hud->updateScore(m_nScore);
    m_hud->showMessage("Get Ready!");
}

void Main::onScroeTimerTimeout()
{
    m_nScore += 1;
    m_hud->updateScore(m_nScore);
}

void Main::onStartTimerTimeout()
{
    m_scoreTimer->start();
    m_mobTimer->start();
}

void Main::onMobTimerTimeout()
{
    Node2D *mob = cast_to<Node2D>(m_mobScene->instantiate()); // 先转换mob到Node2D类型
    PathFollow2D *mobSpawnlocation = m_mobSpawnLocation;
    RandomNumberGenerator *rng = memnew(RandomNumberGenerator);
    mobSpawnlocation->set_progress_ratio(rng->randf());
    mob->set_position(mobSpawnlocation->get_position());
    double n_direction = mobSpawnlocation->get_rotation() + M_PI / 2;
    n_direction += rng->randf_range(-M_PI / 4, M_PI / 4);
    mob->set_rotation(n_direction);
    Vector2 velocity = Vector2(rng->randf_range(150.0, 250.0), 0.0);
    cast_to<RigidBody2D>(mob)->set_linear_velocity(velocity.rotated(n_direction));// 设置mob速度,实现他们向前冲
    add_child(cast_to<Node>(mob));
    memdelete(rng);
    rng = nullptr;
}