module;

#include <__fwd/string.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

module soundManager;

namespace soundManager {
    SoundManager::SoundManager() {
        loadSound("move", "resources/sounds/move.wav");
        loadSound("capture", "resources/sounds/capture.wav");
        loadSound("promote", "resources/sounds/promote.wav");
    }

    void SoundManager::playSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            it->second.play();
        }
    }

    void SoundManager::loadSound(const std::string& name, const std::string& filePath) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filePath)) {
            buffers[name] = buffer;
            sf::Sound sound;
            sound.setBuffer(buffers[name]);
            sounds[name] = sound;
        }
    }
}
