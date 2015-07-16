#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED

#include "../mapObject.h"
#include <vector>
#include "../util/utils.h"
#include <string>
#include "../magic/command.h"

using namespace Magic;
class Mob : public MapObject
{
    public:
        Mob();
        Mob(MobType);
        virtual ~Mob();

        virtual MobType type(void) const;
        Location location(int, int);
        Location location(const Location*);
        int x(int);
        int x(void) const;
        int y(int);
        int y(void) const ;
        bool isSeen(const Mob*);
        int rangeOfSight(int);
        int rangeOfSight(void) const;

        int changeStamina(int);
        int stamina(int);
        const int stamina(void) const;
        const int maxStamina(void) const;

        double changeSpeed(double);
        double speed(void) const;
        double defaultSpeed(void) const;

        int selectedCommandIndex(int);
        int selectedCommandIndex(void) const;
        Command* selectedCommand(void) const;
        const std::string portraitFileName(void) const;
        std::vector<Command*> commands(void) const;
        std::vector<Command>* spells(void);
        const std::vector<Command>* spells(void) const;
        int unblockTime(void) const;
        bool isBlocked(void) const;

        void block(int);
        bool tryUnblock(int);
        void unblock(void);
        void endCombat(void);

    protected:
        std::string portraitFileName(std::string);
        std::vector<Command>* spellCommands(void);
        std::vector<Command>* otherCommands(void);
        double defaultSpeed(double);

        static std::vector<Command*> toPointers(const std::vector<Command>& source);

    private:
        MobType _type;
        int _x;
        int _y;
        int _stamina;
        int _maxStamina;
        int _rangeOfSight;
        int _selectedCommandIndex = 0;
        int _unblockTime = 0;
        std::vector<Command> _spellCommands;
        std::vector<Command> _otherCommands;
        std::string _portraitFileName;
        double _defaultSpeed = 1.0;
        double _speedMultiplier = 1.0;
        bool _isBlocked = false;



};
#endif
