#ifndef ACTION_TRANSMUTER_HH
#define ACTION_TRANSMUTER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionTransmuter : public rules::Action<GameState>
{
public:
    ActionTransmuter(position pos, int player_id)
        : pos_(pos)
        , player_id_(player_id)
    {
    }
    ActionTransmuter() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_TRANSMUTER; }

private:
    position pos_;
    int player_id_;
};

#endif // !ACTION_TRANSMUTER_HH
