#include "viewManagerMenuToolsBase.h"
#include "../play/party.h"
#include "../util/assetCache.h"
using namespace View;

const SDL_Rect ViewManagerMenuToolsBase::_menuControl = SDL_Rect { 6, 6, 150, 50 };
const int ViewManagerMenuToolsBase::cursorXOffset = borderWidth + _menuControl.x;
const int ViewManagerMenuToolsBase::cursorYOffset = borderWidth + _menuControl.y;

void ViewManagerMenuToolsBase::renderPCs(
    const Party& party, int memberIndex, const SDL_Rect& vP,
    int selectedPosition, bool showGains
)
{
    bool isOrdering = selectedPosition >= 0 && memberIndex >= 0;
    SDL_Rect templateRect{ vP.x + cursorXOffset, vP.y, vP.w - cursorXOffset, 160 };
    for (natural i = 0; i < party.members().size(); i++)
    {
        natural position = i;
        natural uMemberIndex = memberIndex;
        natural uSelectedPosition = selectedPosition;
        if (isOrdering)
        {
            if (uMemberIndex == i)
            {
                position = uSelectedPosition;
            }
            else
            {
                if (uSelectedPosition <= i && i <= uMemberIndex)
                    position = i + 1;
                else if (uMemberIndex <= i && i <= uSelectedPosition)
                    position = i - 1;
                else
                    position = i;
            }
        }

        SDL_Rect rect
        {
            templateRect.x + ((position == uSelectedPosition && isOrdering) ? cursorXOffset : 0),
            templateRect.y + (templateRect.h * int(position)),
            templateRect.w,
            templateRect.h
        };

        PC* pc = party.memberAt(i);
        const int TEXT_WIDTH = 22;
        const int TEXT_HEIGHT = 19;
        const int ICON_SIZE = 45;
        const int REL_TEXT_Y = (ICON_SIZE / 2) - (TEXT_HEIGHT / 2);
        const int LABEL_GAP = 30;
        const int MARGIN_Y = 10;

        const SDL_Colour* colour;

        if (memberIndex == int(i))
            colour = &SELECTED_COLOUR;
        else
            colour = &TEXT_COLOUR;

        // Draw name;
        std::string name = pc->name();
        SDL_Rect nameRect {rect.x + 2, rect.y + 3, TEXT_WIDTH * int(name.length()), TEXT_HEIGHT };
        SDL_Texture* textTure = formatFontTexture(name, colour);
        SDL_RenderCopy(renderer(), textTure, NULL, &nameRect);

        // Draw Portrait
        SDL_Rect portRect { rect.x, nameRect.y + nameRect.h, 100,100 };
        SDL_RenderCopy(renderer(), assets()->get(pc->portraitFileName()), NULL, &portRect);

        // Draw class name
        std::string className = pc->className();
        SDL_Rect classRect { rect.x + 2, portRect.y + portRect.h + TEXT_HEIGHT - 10, TEXT_WIDTH * int(className.length()), TEXT_HEIGHT};
        textTure = formatFontTexture(className, colour);
        SDL_RenderCopy(renderer(), textTure, NULL, &classRect);

        // Draw stamina.
        SDL_Rect stmLabelRect { rect.x + TEXT_WIDTH * 12, rect.y + 3, ICON_SIZE, ICON_SIZE};

        SDL_RenderCopy(renderer(), assets()->get(RESOURCE_LOCATION + "stamina.png"), NULL, &stmLabelRect);

        // Draw skill.
        std::string value = displayMultiplier(pc->defaultSkill());
        SDL_Rect sklLabelRect { stmLabelRect.x, stmLabelRect.y + stmLabelRect.h + MARGIN_Y, ICON_SIZE, ICON_SIZE};
        SDL_Rect sklValueRect { sklLabelRect.x + sklLabelRect.w + LABEL_GAP, sklLabelRect.y + REL_TEXT_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };

        SDL_RenderCopy(renderer(), assets()->get(RESOURCE_LOCATION + "skill.png"), NULL, &sklLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &sklValueRect);

        // Draw resistance
        value = displayMultiplier(pc->defaultResistance());
        SDL_Rect resLabelRect { sklLabelRect.x, sklLabelRect.y + sklLabelRect.h + MARGIN_Y, ICON_SIZE, ICON_SIZE};
        SDL_Rect resValueRect { sklValueRect.x, resLabelRect.y + REL_TEXT_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), assets()->get(RESOURCE_LOCATION + "resistance.png"), NULL, &resLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &resValueRect);

        // Draw Speed
        value = displayMultiplier(pc->defaultSpeed());
        SDL_Rect spdLabelRect { sklValueRect.x + sklValueRect.w + 4 * LABEL_GAP, sklLabelRect.y, ICON_SIZE, ICON_SIZE};
        SDL_Rect spdValueRect { spdLabelRect.x + spdLabelRect.w + LABEL_GAP, spdLabelRect.y + REL_TEXT_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), assets()->get(RESOURCE_LOCATION + "speed.png"), NULL, &spdLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &spdValueRect);

        // Draw Defense
        value = displayMultiplier(pc->defaultDefence());
        SDL_Rect defLabelRect { spdLabelRect.x, spdLabelRect.y + spdLabelRect.h + MARGIN_Y, ICON_SIZE, ICON_SIZE};
        SDL_Rect defValueRect { spdValueRect.x, defLabelRect.y + REL_TEXT_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), assets()->get(RESOURCE_LOCATION + "defence.png"), NULL, &defLabelRect);
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &defValueRect);

        value = std::to_string(pc->stamina()) + "/" + std::to_string(pc->maxStamina());
        SDL_Rect stmValueRect { sklValueRect.x, stmLabelRect.y + REL_TEXT_Y, TEXT_WIDTH * int(value.length()), TEXT_HEIGHT };
        SDL_RenderCopy(renderer(), formatFontTexture(value, colour), NULL, &stmValueRect);

        if (showGains)
        {
            const std::string prefix = "+ ";
            std::string gainAsString = "";
            int gain = 0;
            SDL_Rect gainRect;
            gain = pc->getStaminaGain();
            if (gain > 0)
            {
                gainAsString = prefix + std::to_string(gain);
                gainRect = SDL_Rect{ stmValueRect.x + stmValueRect.w + LABEL_GAP, stmValueRect.y, TEXT_WIDTH * int(gainAsString.length()), TEXT_HEIGHT };
                SDL_RenderCopy(renderer(), formatFontTexture(gainAsString, colour), NULL, &gainRect);
            }

            gain = pc->getSkillGain();
            if (gain > 0)
            {
                gainAsString = prefix + std::to_string(gain);
                gainRect = SDL_Rect{ sklValueRect.x + sklValueRect.w + LABEL_GAP, sklValueRect.y, TEXT_WIDTH * int(gainAsString.length()), TEXT_HEIGHT };
                SDL_RenderCopy(renderer(), formatFontTexture(gainAsString, colour), NULL, &gainRect);
            }

            gain = pc->getSpeedGain();
            if (gain > 0)
            {
                gainAsString = prefix + std::to_string(gain);
                gainRect = SDL_Rect{ spdValueRect.x + spdValueRect.w + LABEL_GAP, spdValueRect.y, TEXT_WIDTH *  int(gainAsString.length()), TEXT_HEIGHT };
                SDL_RenderCopy(renderer(), formatFontTexture(gainAsString, colour), NULL, &gainRect);
            }

            gain = pc->getResistanceGain();
            if (gain > 0)
            {
                gainAsString = prefix + std::to_string(gain);
                gainRect = SDL_Rect{ resValueRect.x + resValueRect.w + LABEL_GAP, resValueRect.y, TEXT_WIDTH * int(gainAsString.length()), TEXT_HEIGHT };
                SDL_RenderCopy(renderer(), formatFontTexture(gainAsString, colour), NULL, &gainRect);
            }

            gain = pc->getDefenceGain();
            if (gain > 0)
            {
                gainAsString = prefix + std::to_string(gain);
                gainRect = SDL_Rect{ defValueRect.x + defValueRect.w + LABEL_GAP, defValueRect.y, TEXT_WIDTH * int(gainAsString.length()), TEXT_HEIGHT };
                SDL_RenderCopy(renderer(), formatFontTexture(gainAsString, colour), NULL, &gainRect);
            }
        }

        drawBorder(rect, 3, colour, false);
    }
}
