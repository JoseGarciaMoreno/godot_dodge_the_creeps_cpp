#ifndef MAIN_H
#define MAIN_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>
#include "player.h"
#include <godot_cpp/classes/marker2d.hpp>
#include <godot_cpp/classes/path2d.hpp>
#include <godot_cpp/classes/path_follow2d.hpp>
#include "hud.h"
#include <godot_cpp/classes/audio_stream_player2d.hpp>

namespace godot
{
    class Main : public Node
    {
        GDCLASS(Main, Node)

    private:
        Ref<PackedScene> m_mobScene;
        Ref<PackedScene> m_mob;
        Timer *m_scoreTimer, *m_mobTimer, *m_startTimer;
        int m_nScore;
        Player *m_player;
        Marker2D *m_startPosition;
        Path2D *m_mobPath;
        PathFollow2D *m_mobSpawnLocation;
        Hud *m_hud;
        AudioStreamPlayer2D *m_music, *m_deathSound ;
        
    protected:
        static void _bind_methods();

    public:
        Main();
        ~Main();
        void _ready() override;
        void set_mob_scene(const godot::Ref<godot::PackedScene> &p_scene);
        Ref<godot::PackedScene> get_mob_scene() const;
        void gameOver();
        void newGame();
        void onScroeTimerTimeout();
        void onMobTimerTimeout();
        void onStartTimerTimeout();
    };
}

#endif