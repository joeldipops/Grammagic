#include "combatManager.h"
#include "../view/controlViewManager.h"
#include "../view/mapViewManager.h"
#include "../view/miniMapViewManager.h"
#include "../view/statsViewManager.h"
#include "../view/victoryViewManager.h"

using namespace Magic;

CombatManager::CombatManager(SDL_Renderer* r, AssetCache* a, View::ScreenViewContainer views) : StateManager(r, a)
{
    using namespace View;
    static SDL_Rect wholeScreen = { 0, 0, WIDTH, HEIGHT};
    _controlView = (ControlViewManager*) views.Top;
    _statsView = (StatsViewManager*) views.Right;
    _miniMapView = (MiniMapViewManager*) views.TopRight;
    _mapView = (MapViewManager*) views.Main;

    _victoryView = new VictoryViewManager(r, wholeScreen, a);
}

CombatManager::~CombatManager(void)
{
    deletePtr(_victoryView);
};

Play::PlayState CombatManager::start(void)
{
    return Play::PlayState::Exit;
}

Play::PlayState CombatManager::start(GameMap* map_)
{
    state(Play::PlayState::Combat);

    _map = map_;
    SDL_Event event;
    _field = BattleField(map_);

    _field.isInCombat(true);
    bool rerender = true;

    while(_field.isInCombat())
    {
        Util::sleep(50);

        buryTheDead();

        if (rerender)
            render();

        // Always rerender for now, at least until I can get the clocks working prettily
        rerender = true;

        std::vector<SDL_Event> events;
        Event myEvent;
        while(eventManager.pollEvent(&myEvent))
        {
            SDL_Event event = *myEvent.InnerEvent;
            if (event.type == SDL_QUIT)
                return Play::PlayState::Exit;
            events.push_back(event);
        }

        std::vector<Mob*> due = _field.getDue();
        for(Mob* combatant : due)
        {
            if (combatant->type() == MobType::PlayerCharacter)
            {
                int index = map_->party()->getIndexOf(combatant);
                if (index < 0)
                    continue;
                if (_selectedMemberIndex < 0)
                    _selectedMemberIndex = index;
                if (_selectedMemberIndex == index)
                    rerender = processPcTurn(combatant, _field, &events);
            }
            else if (combatant->type() == MobType::Hostile)
                rerender = processHostileTurn((Enemy*)combatant, _field);
        }

        if (_field.isDefeat())
            state(Play::PlayState::GameOver);
    }

    endCombat();

    _map = nullptr;
    return result();
}

void CombatManager::endCombat(void)
{
    buryTheDead();

    if (state() == PlayState::Combat)
    {
        _field.endCombat();
        state(PlayState::Movement);
        return;
    }

    // TODO
    if (state() == PlayState::GameOver)
    {
        _field.endCombat();
        return;
    }

    enum debriefStates { START, REVIEW, CALCULATE, DONE };
    debriefStates debriefState = debriefStates::REVIEW;
    debriefStates oldState = debriefStates::START;
    while(debriefState != debriefStates::DONE)
    {
        if (oldState != debriefState)
        {
            render();
            oldState = debriefState;
        }

        Util::sleep(50);

        Event myEvent;
        while(eventManager.pollEvent(&myEvent))
        {
            SDL_Event event = *myEvent.InnerEvent;
            if (event.type == SDL_QUIT)
                state(Play::PlayState::Exit);
            if (event.type != SDL_KEYDOWN)
                continue;

            switch(event.key.keysym.sym)
            {
                case SDLK_q:
                    state(Play::PlayState::Exit);
                    return;
                default:
                    if (debriefState == debriefStates::REVIEW)
                    {
                        debriefState = debriefStates::CALCULATE;
                        _field.endCombat();
                    }
                    else if (debriefState == debriefStates::CALCULATE)
                    {
                        debriefState = debriefStates::DONE;
                    }
                    break;
            }
        }
    }
}

/**
 * Cleans up mobs whose stamina is <= 0
 */
void CombatManager::buryTheDead(void)
{
    // If selected pc is out of it, assign a new leader.
    if (_selectedMemberIndex >= int(_map->party()->members().size())
    || (_selectedMemberIndex >= 0
    &&  _map->party()->memberAt(_selectedMemberIndex)->stamina() <= 0
    ))
        _selectedMemberIndex = -1;

    _map->party()->getSpoils(_field);
    _field.buryTheDead();
}

void CombatManager::render(void)
{
    SDL_SetRenderDrawColor(renderer(), 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer());

    switch (state())
    {
        case PlayState::Combat:
            _mapView->render(_map, state());
            if (_selectedMemberIndex >= 0)
                _controlView->render(_map->party()->memberAt(_selectedMemberIndex), state());
            else
                _controlView->render(nullptr, PlayState::Combat);
            _statsView->render(*_map, state(), _selectedMemberIndex);
            _miniMapView->render();
            break;
        case PlayState::Victory:
            _victoryView->render(*_map->party());
            break;
        case PlayState::GameOver:
        default:
            break;
    }

    SDL_RenderPresent(renderer());
}

bool CombatManager::moveCursor(Mob* mob, Core::InputPress input)
{
    int index = mob->selectedCommandIndex();
    int noOfCommands= mob->commands().size();
    int newIndex = StateManager::moveCursor(input, index, noOfCommands, _controlView->menuItemsPerColumn());

    if (index == newIndex)
        return false;

    return (mob->selectedCommandIndex(newIndex) != -1);
}

bool CombatManager::processHostileTurn(Enemy* mob, BattleField& field)
{
    mob->aiAct(field);
    return true;
}

bool CombatManager::processPcTurn(Mob* pc, BattleField& field, std::vector<SDL_Event>* events)
{

    bool hasUpdate;

    // If there
    if (events->size() <= 0)
        return true;
    while(events->size() > 0)
    {
        SDL_Event event = events->back();
        events->pop_back();
        if (event.type != SDL_KEYDOWN)
            continue;

        switch(event.key.keysym.sym)
        {
            case SDLK_w:
                hasUpdate |= moveCursor(pc, Core::InputPress::UP);
                break;
            case SDLK_a:
                hasUpdate |= moveCursor(pc, Core::InputPress::LEFT);
                break;
            case SDLK_s:
                hasUpdate |= moveCursor(pc, Core::InputPress::DOWN);
                break;
            case SDLK_d:
                hasUpdate |= moveCursor(pc, Core::InputPress::RIGHT);
                break;
            case SDLK_RIGHTBRACKET:
                hasUpdate |= processCommand(pc, field);
                _selectedMemberIndex = -1;
                break;
            case SDLK_RETURN:
                // Pause
                break;
        }
    }

    return hasUpdate;
}

bool CombatManager::processCommand(Mob* mob, BattleField& field)
{
    // Carry out the command
    int waitMs = mob->selectedCommand()->execute(mob, field);

    // Apply the duration - mob cannot act again until command completes.
    mob->block(SDL_GetTicks() + waitMs);

    if (field.isDefeat())
    {
        state(Play::PlayState::GameOver);
        return true;
    }
    if (field.isVictory())
        state(Play::PlayState::Victory);

    return true;
}

Play::PlayState CombatManager::result(void) const
{
    return StateManager::state();
}

Play::PlayState CombatManager::state(Play::PlayState state_)
{
    StateManager::state(state_);
    switch(state_)
    {
        case Play::PlayState::GameOver:
        case Play::PlayState::Victory:
            _field.isInCombat(false);
            break;
        default: _field.isInCombat(true);
            break;
    }
    return StateManager::state();
}


