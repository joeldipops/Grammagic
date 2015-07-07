#include "assetCache.h"
#include <iostream>

AssetCache::AssetCache() {}

/**
 * Constructor
 */
AssetCache::AssetCache(SDL_Renderer* renderer)
{
    _renderer = renderer;
    _assets = std::map<std::string, SDL_Texture*>();
    _fontAssets = std::map<std::string, TTF_Font*>();
}

/**
 * Destructor
 */
AssetCache::~AssetCache(void)
{
        emptyCache();
}

/**
 * Loads an asset, either, from file, or from the cache.
 * @param fileName The filename of the asset.
 * @return The asset in memory.
 */
SDL_Texture* AssetCache::get(std::string fileName)
{
    // If asset is already in the cache, just return it.
    if (_assets.count(fileName) > 0)
        return _assets.at(fileName);

    // Otherwise, load the image from disc.
    SDL_Surface* temp = IMG_Load(fileName.c_str());
    if( temp == NULL )
        std::cout << "Unable to load image " << IMG_GetError() << std::endl;

    SDL_Texture* result = SDL_CreateTextureFromSurface( _renderer, temp);
    SDL_FreeSurface(temp);

    _assets[fileName] = result;
    return result;
}

/**
 * Loads a TTF font type asset either from file or from the cache.
 * @param fileName The name of path of the file on disc.
 * @param fontSize the pixel size of the font.
 * @return The font.
 */
TTF_Font* AssetCache::get(std::string fileName, int fontSize)
{
    std::string key = fileName + std::to_string(fontSize);
    if(_fontAssets.count(key) > 0)
        return _fontAssets.at(key);

    TTF_Font* font = TTF_OpenFont(fileName.c_str(), fontSize);
    if (font == NULL)
        std::cout << "Unable to load font " << TTF_GetError() << std::endl;

    _fontAssets[key] = font;
    return font;
}

/**
 * Removes a specific asset from the cache.
 * @param key The key (filename) of the asset.
 */
void AssetCache::discard(std::string key)
{
    SDL_DestroyTexture(_assets[key]);
    _assets[key] = nullptr;
    _assets.erase(key);

    _fontAssets[key] = nullptr;
    _fontAssets.erase(key);
}

/**
 * Invalidates the entire cache, freeing up memory.
 */
void AssetCache::emptyCache(void)
{
    for(auto &item : _assets)
    {
        SDL_DestroyTexture(item.second);
        item.second = nullptr;
    }
}
