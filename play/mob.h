#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED

#include <vector>
#include <string>

#include "../mapObject.h"
#include "../util/utils.h"
#include "../magic/command.h"
#include "combatable.h"

using namespace Magic;
class Mob : public MapObject, public Combatable
{
    public:
        Mob();
        Mob(MobType);
        virtual ~Mob();

        virtual MobType type(void) const;


        bool isSeen(const MapObject&) const;
        bool isSensed(const MapObject&) const;
        bool isMob(void) const { return true; };
        virtual bool isPlayerControlled(void) const { return false; };

        // Stats
        int rangeOfSense(int);
        int rangeOfSense(void) const;
        int rangeOfSight(int);
        int rangeOfSight(void) const;

        int changeStamina(int);
        int stamina(int);
        int stamina(void) const;
        int maxStamina(void) const;

        float changeSpeed(float);
        float speed(void) const;
        float defaultSpeed(void) const;

        float changeResistance(float);
        float resistance(void) const;
        float defaultResistance(void) const;

        float changeDefence(float);
        float defence(void) const;
        float defaultDefence(void) const;

        float changeSkill(float);
        float skill(void) const;
        float defaultSkill(void) const;

        int selectedCommandIndex(unsigned int);
        int selectedCommandIndex(void) const;
        Command* selectedCommand(void) const;
        const std::string portraitFileName(void) const;
        std::vector<Command*> commands(void) const;
        std::vector<Command>* spells(void);
        const std::vector<Command>* spells(void) const;
        int unblockTime(void) const;
        bool isBlocked(void) const;
        int elapsedWait(void) const;

        void block(int);
        bool tryUnblock(int);
        void unblock(void);
        void endCombat(void);

    protected:
        std::string portraitFileName(std::string);
        std::vector<Command>* spellCommands(void);
        std::vector<Command>* otherCommands(void);

        float defaultSpeed(float);
        float defaultResistance(float);
        float defaultDefence(float);
        float defaultSkill(float);

        static std::vector<Command*> toPointers(const std::vector<Command>& source);
        bool isInRange(const MapObject&, int value) const;

    private:
        MobType _type;

        int _stamina;
        int _maxStamina;
        int _rangeOfSight;
        int _rangeOfSense;
        int _selectedCommandIndex = 0;

        int _unblockTime = 0;
        int _blockedTime = 0;
        bool _isBlocked = false;

        std::vector<Command> _spellCommands;
        std::vector<Command> _otherCommands;
        std::string _portraitFileName;

        // Base stats.
        float _defaultSpeed = 1.0;
        float _defaultResistance = 1.0;
        float _defaultDefence = 1.0;
        float _defaultSkill = 1.0;

        // Tempoprary stats.
        float _speedMultiplier = 1.0;
        float _resistanceMultiplier = 1.0;
        float _defenceMultiplier = 1.0;
        float _skillMultiplier = 1.0;
};
#endif
