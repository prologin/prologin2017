#ifndef ACTION_CATALYSER_HH
#define ACTION_CATALYSER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionCatalyser : public rules::Action<GameState>
{
public:
    ActionCatalyser(position pos, int id_apprenti, case_type terrain,
                    int player_id)
        : pos_(pos)
        , id_apprenti_(id_apprenti)
        , terrain_(terrain)
        , player_id_(player_id)
    {
    }
    ActionCatalyser() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos_);
        buf.handle(id_apprenti_);
        buf.handle(terrain_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_CATALYSER; }

private:
    position pos_;
    int id_apprenti_;
    case_type terrain_;
    int player_id_;
};

#endif // !ACTION_CATALYSER_HH
