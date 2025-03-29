#include "hud.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

using namespace godot;

void Hud::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("awaitMessageTimerTimeout"), &Hud::awaitMessageTimerTimeout);
    ClassDB::bind_method(D_METHOD("getTreeTimeout"), &Hud::getTreeTimeout);
    ClassDB::bind_method(D_METHOD("onStartButtonPressed"), &Hud::onStartButtonPressed);
    ClassDB::bind_method(D_METHOD("onMessageTimerTimeout"), &Hud::onMessageTimerTimeout);
    ADD_SIGNAL(MethodInfo("start_game"));
}
void Hud::showMessage(StringName text)
{
    m_message->set_text(text);
    m_message->show();
    m_messageTimer->start();
}

void Hud::_ready()
{
    m_message = get_node<Label>("Message");
    m_messageTimer = get_node<Timer>("MessageTimer");
    m_startButton = get_node<Button>("StartButton");
    m_startButton->connect("pressed", Callable(this, "onStartButtonPressed"), CONNECT_PERSIST);
    m_messageTimer->connect("timeout", Callable(this, "onMessageTimerTimeout"), CONNECT_PERSIST);
    m_scoreLabel = get_node<Label>("ScoreLabel");
}

void Hud::showGameOver()
{
    showMessage("Game Over");
    m_messageTimer->connect("timeout", Callable(this, "awaitMessageTimerTimeout"), CONNECT_ONE_SHOT);
    m_messageTimer->start();
    Ref<SceneTreeTimer> timer = get_tree()->create_timer(2.0);
    timer->connect("timeout", Callable(this, "getTreeTimeout"), CONNECT_PERSIST);
}

void Hud::awaitMessageTimerTimeout()
{
    m_message->set_text("Dodge the Creeps!");
    m_message->show();
    m_messageTimer->stop();
}

void Hud::getTreeTimeout()
{
    m_startButton->show();
}

void Hud::onStartButtonPressed()
{
    m_startButton->hide();
    emit_signal("start_game");
}

void Hud::onMessageTimerTimeout()
{
    m_message->hide();
}

void Hud::updateScore(int score)
{
    m_scoreLabel->set_text(String::num(score, 0));
}
