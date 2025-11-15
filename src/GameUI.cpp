// GameUI.cpp
#include "../include/GameUI.h"
#include <iostream> // Để debug

// Hàm khởi tạo (dùng danh sách khởi tạo)
GameUI::GameUI() :
    m_window(sf::VideoMode(600, 600), "Go Game 9x9"), // Kích thước cửa sổ
    m_game(9), // Khởi tạo logic Game 9x9 (như trong Game.h)
    m_gameView(m_game.getBoard()) // Truyền Board từ m_game cho m_gameView
{
    m_window.setFramerateLimit(60);
}

// Hàm chạy chính
void GameUI::run() {
    while (m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// Xử lý Input
void GameUI::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        // BẮT SỰ KIỆN CLICK CHUỘT
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                
                // 1. Lấy vị trí click
                sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
                int boardX, boardY;

                // 2. Nhờ "Họa sĩ" chuyển đổi
                if (m_gameView.mapPixelToBoardCoords(pixelPos, boardX, boardY)) {
                    
                    // 3. Nếu hợp lệ, bảo "Bộ não" thực hiện nước đi
                    std::cout << "Clicked: " << boardX << ", " << boardY << std::endl;
                    
                    // GỌI HÀM makeMove TỪ GAME.CPP CỦA BẠN
                    bool success = m_game.makeMove(boardX, boardY);
                    
                    if (!success) {
                        std::cout << "Nuoc di KHONG HOP LE!" << std::endl;
                    }
                    // m_game.makeMove đã tự động đổi lượt
                    // và xử lý ăn quân

                    if (game.getGameState() == GameState::FINISHED){
                        // vẽ bảng show winner và điểm của cả 2 
                    }
                }
            }
        }
    }
}

// Cập nhật logic (không cần cho cờ vây)
void GameUI::update() {
    // (Có thể dùng để AI tính toán sau này)
}

// Vẽ
void GameUI::render() {
    m_window.clear(sf::Color(220, 179, 92)); // Màu bàn cờ gỗ

    // Chỉ cần bảo "Họa sĩ" vẽ
    m_gameView.draw(m_window);

    m_window.display();
}