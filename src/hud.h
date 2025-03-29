#ifndef HUD_H
#define HUD_H

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/button.hpp>

namespace godot
{
    class Hud : public CanvasLayer
    {
        GDCLASS(Hud, CanvasLayer)

    private:
        Label *m_message, *m_scoreLabel;
        Timer *m_messageTimer;
        Button *m_startButton;

    protected:
        static void _bind_methods();

    public:
        void showMessage(StringName text);
        void _ready() override;
        void showGameOver();
        void awaitMessageTimerTimeout();
        void getTreeTimeout();
        void updateScore(int score);
        void onStartButtonPressed();
        void onMessageTimerTimeout();
    };
}

#endif