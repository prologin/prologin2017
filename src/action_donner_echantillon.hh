#ifndef ACTION_DONNER_ECHANTILLON_HH
#define ACTION_DONNER_ECHANTILLON_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionDonnerEchantillon : public rules::Action<GameState>
{
public:
    ActionDonnerEchantillon(echantillon echantillon_donne, int player_id)
        : echantillon_donne_(echantillon_donne)
        , player_id_(player_id)
    {
    }
    ActionDonnerEchantillon() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(echantillon_donne_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_DONNER_ECHANTILLON; }

private:
    echantillon echantillon_donne_;
    int player_id_;
};

#endif // !ACTION_DONNER_ECHANTILLON_HH
