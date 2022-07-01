#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
// struct node{
//   int data;
//   struct
// };


class Window {
  public: 
    Window(std::string title, int x, int y, int w, int h) {
      m_title = title;
      m_x = x;
      m_y = y;
      m_w = w;
      m_h = h;

      m_shape = new sf::RectangleShape;
      m_shape->setPosition(x, y);
      m_shape->setSize(sf::Vector2f((float) w, (float) h));
      m_shape->setFillColor(sf::Color(255, 0, 0, 100));

      m_font = new sf::Font;
      if (!m_font->loadFromFile("Roboto-Medium.ttf")) {
        printf("Gagal load file font\n");
        exit(1);
      }

      m_text = new sf::Text;
      m_text->setString(m_title);
      m_text->setFont(*m_font);
      m_text->setCharacterSize(16);
      m_text->setPosition(m_x + 8, m_y + 8);
      m_text->setFillColor(sf::Color::White);
    }

    ~Window() {
      printf("Window diclose\n");
    }

    void Draw(sf::RenderWindow& window) {
      window.draw(*m_shape);
      window.draw(*m_text);
    }

    sf::Vector2f GetNodePosition() { 
      return sf::Vector2f(m_x + m_w, m_y + m_h); 
    }

    sf::Vector2f GetNodeBack() { 
      return sf::Vector2f(m_x, m_y); 
    }

  private:
    int m_x, m_y, m_w, m_h;
    std::string m_title;
    sf::RectangleShape* m_shape;
    sf::Font* m_font;
    sf::Text* m_text;
};

struct node {
  char* title;
  Window* window;
  struct node* next;
};

typedef struct node* node_t;
node_t head = NULL;

void insert(char* nama, int data, int x, int y, int w, int h) {
  node_t baru = (node_t) malloc(sizeof(node));
  Window* win = new Window(nama, x, y, w, h);
  
  baru->title = nama;
  baru->window = win;
  baru->next = NULL;

  if (head == NULL) {
    head = baru;
  } else {
    node_t i = head;
    while (i->next != NULL) {
      i = i->next;
    }

    // i = baru;
    i->next = baru;
  }
}

void print(sf::RenderWindow& window) {
  std::vector<sf::RectangleShape> lines; 
  int count = 0;
  
  if (head == NULL) {
    // std::cout << "Nggak ada isinya" << std::endl;
    printf("Nggak ada isinya\n");
  } else {
    int id = 0;
    node_t i = head;
    sf::RectangleShape line;
    int tmp;

    while (i != NULL) {
      if (id > 2) id = 0;
      
      if (i->next != NULL) {
        float size_x = i->next->window->GetNodeBack().x - i->window->GetNodePosition().x;
        int X[2];
        int Y[2];

        X[0] = i->window->GetNodePosition().x;
        X[0] = i->next->window->GetNodePosition().x;
        Y[1] = i->window->GetNodePosition().y;
        Y[1] = i->next->window->GetNodePosition().y;

        // sleep_for(nanoseconds(10));
        // sleep_until(system_clock::now() + seconds(1));
        
        // printf("P1 X: %d, P1 Y: %d\n", X[0], Y[0]);
        // printf("P2 X: %d, P2 y: %d\n", X[1], X[1]);

        line.setSize(sf::Vector2f(size_x, 5.0f));
        line.setPosition(i->window->GetNodePosition());
        line.setFillColor(sf::Color::Black);
        // line.rotate(angleInDegree);
        lines.push_back(line);
      }
      
      i->window->Draw(window);
      i = i->next;

      id++;
      count++;
    }
  }
  
  sf::Vertex linearr[count];
  // int j = 0;

  for (auto i : lines) {
    window.draw(i);
    // j++; 
  }

  linearr->color = sf::Color::Black;
  window.draw(linearr, count, sf::Lines);
}

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

  insert("Window 1", 4, 10, 10, 128, 128);
  insert("Window 2", 4, 128 + 128, 10, 128, 128);
  insert("Window 4", 4, 128 + 128 + 128 + 128, 10 * 8, 128, 128);

  int i = 0;
  sf::Vertex line[2];
  bool clicked = false;

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      // int mouseButtonX = e.mouseButton.x;
      //       int mouseButtonY = event.mouseButton.y;
      //       int mouseMoveX = event.mouseMove.x;
      //       int mouseMoveY = event.mouseMove.y;
      if (e.type == sf::Event::Closed)
        window.close();

      // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
      //   insert("Window x", i, (i * 128) + 32, 256, 128, 128);
      //   i++;
      // }

      // if (e.type == sf::Event::MouseButtonReleased) {
      //   if (sf::Mouse::Right)
      //     insert("Window sekian", 4, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 128, 128);
      // }

      if (e.type == sf::Event::MouseButtonPressed) {
          if (clicked)
            clicked = false;
          else 
            clicked = true;

          printf("Clicked %d\n", clicked);
          line[0] = sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        
      }

      if (clicked)
        line[1] = sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
    }

    line->color = sf::Color::Blue;

    window.clear(sf::Color(255, 255, 255, 255));
    print(window);
    window.draw(line, 2, sf::Lines);
    window.display();
  }

  return 0;
}