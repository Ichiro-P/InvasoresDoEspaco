#include <janela.hpp>
#include <erroManuseio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>

janela::janela(const sf::Vector2u& resolucao, const std::filesystem::path& caminhoIcone, const std::filesystem::path& caminhoFonte, const int vidasIniciais, const int pontosIniciais) 
: vidas(vidasIniciais), pontos(pontosIniciais), resolucao(resolucao), textoVidas(fonte), textoPontos(fonte) {
    if (!fonte.openFromFile(caminhoFonte.string())) erroArquivo(caminhoIcone.string());
    if (!icone.loadFromFile(caminhoIcone.string())) erroArquivo(caminhoFonte.string());
    instanciaJanela.create(sf::VideoMode({resolucao.x, resolucao.y}), L"Invasores do Espaço", sf::Style::Close);
    instanciaJanela.setFramerateLimit(60);
    instanciaJanela.setIcon(icone);

    textoVidas.setString("Vidas: " + std::to_string(vidas));
    textoVidas.setFillColor(sf::Color::White);
    textoVidas.setCharacterSize(static_cast<unsigned int>(resolucao.x / 50));
    textoVidas.setPosition(sf::Vector2f({static_cast<float>(resolucao.x*0.85) , 10.f}));

    textoPontos.setString("Pontos: " + std::to_string(pontos));
    textoPontos.setFillColor(sf::Color::White);
    textoPontos.setCharacterSize(static_cast<unsigned int>(resolucao.x / 50));
    textoPontos.setPosition(sf::Vector2f({20.f , 10.f}));
}

bool janela::getEstado() const& {
    return instanciaJanela.isOpen();
}

void janela::eventos() {
    instanciaJanela.handleEvents(
        [this](const sf::Event::Closed) { instanciaJanela.close(); },
        [this](const sf::Event::KeyPressed tecla) {
            switch (tecla.scancode) {
            case sf::Keyboard::Scancode::Escape:
                instanciaJanela.close();
                break;
            
            default:
                break;
            }
        }
    );
}

void janela::desenhar() {
    instanciaJanela.clear();
    instanciaJanela.draw(textoVidas);
    instanciaJanela.draw(textoPontos);
    instanciaJanela.display();
}


