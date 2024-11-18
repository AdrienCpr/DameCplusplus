module;

#include <SFML/Audio.hpp>
#include <map>
#include <string>

export module soundManager;

export namespace soundManager {

    class SoundManager {
    public:
        SoundManager();

        void playSound(const std::string& name);

    private:
        std::map<std::string, sf::SoundBuffer> buffers;
        std::map<std::string, sf::Sound> sounds;

        void loadSound(const std::string& name, const std::string& filePath);
    };
}
