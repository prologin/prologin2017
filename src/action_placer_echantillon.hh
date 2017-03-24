#ifndef ACTION_PLACER_ECHANTILLON_HH
#define ACTION_PLACER_ECHANTILLON_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionPlacerEchantillon : public rules::Action<GameState>
{
public:
    ActionPlacerEchantillon(position pos1, position pos2, int player_id)
        : pos1_(pos1)
        , pos2_(pos2)
        , player_id_(player_id)
    {
    }
    ActionPlacerEchantillon() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos1_);
        buf.handle(pos2_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_PLACER_ECHANTILLON; }

private:
    position pos1_;
    position pos2_;
    int player_id_;
};

#endif // !ACTION_PLACER_ECHANTILLON_HH
