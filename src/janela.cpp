#include <janela.hpp>
#include <jogador.hpp>
#include <erroManuseio.hpp>
#include <SFML/Graphics.hpp>
#include <gerenciadorInimigos.hpp>
#include <filesystem>
#include <optional>
#include <enums.hpp>
#include <string>

janela::janela(const sf::Vector2u& resolucao, const std::filesystem::path& caminhoIcone, const std::filesystem::path& caminhoFonte, const int vidasIniciais, const int pontosIniciais, const int qps) 
: vidas(vidasIniciais), pontos(pontosIniciais), resolucao(resolucao), textoVidas(fonte), textoPontos(fonte), qps(qps) {
    if (!fonte.openFromFile(caminhoFonte.string())) erroArquivo(caminhoIcone.string());
    if (!icone.loadFromFile(caminhoIcone.string())) erroArquivo(caminhoFonte.string());
    instanciaJanela.create(sf::VideoMode({resolucao.x, resolucao.y}), L"Invasores do Espaço", sf::Style::Close);
    instanciaJanela.setFramerateLimit(qps);
    instanciaJanela.setIcon(icone);

    textoVidas.setString("Vidas: " + std::to_string(vidas));
    textoVidas.setFillColor(sf::Color::White);
    textoVidas.setCharacterSize(static_cast<unsigned int>(resolucao.x / 40));
    textoVidas.setPosition(sf::Vector2f({resolucao.x*0.85f , 10.f}));

    textoPontos.setString("Pontos: " + std::to_string(pontos));
    textoPontos.setFillColor(sf::Color::White);
    textoPontos.setCharacterSize(static_cast<unsigned int>(resolucao.x / 40));
    textoPontos.setPosition(sf::Vector2f({resolucao.x*0.01f , 10.f}));
}

bool janela::getEstado() const& {
    return instanciaJanela.isOpen();
}

void janela::restaurar() {
    vidas = 3;
    pontos = 0;
}

void janela::setPontuacao(const enums::tipo tipo) {
    switch (tipo) {
        case enums::tipo::circulo:
        pontos += 10;
        break;
        case enums::tipo::quadrado:
        pontos += 20;
        break;
        case enums::tipo::triangulo:
        pontos += 40;
        break;
    }
    textoPontos.setString("Pontos: " + std::to_string(pontos));
    if(pontos % 1500 == 0 && vidas < 3 && vidas != 0) {
        textoPontos.setString("Vidas: " + std::to_string(++vidas));
    }
}

void janela::eventos(std::optional<std::reference_wrapper<jogador>> jogador) {
    instanciaJanela.handleEvents(
        [this](const sf::Event::Closed) { instanciaJanela.close(); },
        [this, &jogador](const sf::Event::KeyPressed tecla) {
            if(jogador.has_value()) {
                if(tecla.scancode == sf::Keyboard::Scancode::Escape) instanciaJanela.close();
                if(tecla.scancode == sf::Keyboard::Scancode::Left) jogador->get().mover(enums::direcao::esquerda);
                if(tecla.scancode == sf::Keyboard::Scancode::Right) jogador->get().mover(enums::direcao::direita);
                if(tecla.scancode == sf::Keyboard::Scancode::Space) jogador->get().atirar();
            }
        }
    );
}

void janela::desenhar(const std::optional<std::reference_wrapper<jogador>>& jogador, const std::optional<std::reference_wrapper<gerenciadorInimigos>>& gerenciadorInimigos) {
    instanciaJanela.clear();
    instanciaJanela.draw(textoVidas);
    instanciaJanela.draw(textoPontos);
    if(jogador.has_value()) {
        instanciaJanela.draw(jogador->get().getSprite());
        for(const auto& bala: jogador->get().getBalas()) {
            instanciaJanela.draw(bala.getForma());
        }
    }
    if(gerenciadorInimigos.has_value()) gerenciadorInimigos->get().desenhar(instanciaJanela);
    instanciaJanela.display();
}