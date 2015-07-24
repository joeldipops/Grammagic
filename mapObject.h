#ifndef MAPOBJECT_H_INCLUDED
#define MAPOBJECT_H_INCLUDED

#include <string>
class MapObject
{
    public:
        MapObject(void);
        virtual ~MapObject(void) {};
        bool isDense(void);
        std::string imageFileName(std::string);
        const std::string imageFileName(void) const;

        virtual int changeStamina(int) {return 1;};
        virtual float changeSpeed(float) {return 1;};
        virtual float changeDefence(float) { return 1;};
        virtual float changeResistance(float) { return 1;};
        virtual int stamina(void) const { return 1; };
        virtual float speed(void) const { return 1; };
        virtual float resistance(void) const {return 1; };
        virtual float defence(void) const { return 1; };

        static const std::string RESOURCE_LOCATION;

    protected:
        bool isDense(bool);
    private:
        bool _isDense;
        std::string _imageFileName;

};

#endif
