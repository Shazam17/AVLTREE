#include "pch.h"
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#define Log(x) std::cout << x << std::endl

struct Node
{
	int key;
	int height;
	Node *left;
	Node *right;
};

inline int max(int a, int b) { return (a > b) ? a : b; }

inline int height(Node *N) { return  N == nullprt ? 0 : N->height;  }

inline int getBalance(Node *N) { return N == nullptr ? 0 : height(N->left) - height(N->right); }

Node* newNode(int key)
{
	Node* node = new Node();
	node->key = key;
	node->left = nullptr;
	node->right = nullptr;
	node->height = 1; 
	return(node);
}
Node *rightRotate(Node *y)
{
	Node *x = y->left;
	Node *T2 = x->right;

	// Perform rotation  
	x->right = y;
	y->left = T2;

	// Update heights  
	y->height = max(height(y->left),
		height(y->right)) + 1;
	x->height = max(height(x->left),
		height(x->right)) + 1;

	// Return new root  
	return x;
}

Node *leftRotate(Node *x)
{
	Node *y = x->right;
	Node *T2 = y->left;

	// Perform rotation  
	y->left = x;
	x->right = T2;

	// Update heights  
	x->height = max(height(x->left),
		height(x->right)) + 1;
	y->height = max(height(y->left),
		height(y->right)) + 1;

	// Return new root  
	return y;
}

Node* insert(Node* node, int key)
{
	/* 1. Perform the normal BST insertion */
	if (node == nullptr)
		return(newNode(key));

	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed in BST  
		return node;

	
	node->height = 1 + max(height(node->left),
		height(node->right));
	
	int balance = getBalance(node);

	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case  
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case  
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case  
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

void displayText(int x, int y,int size , sf::Font &font, std::string message, sf::RenderWindow & window)
{
	sf::Text text;
	text.setFont(font);
	text.setString(message);
	text.setPosition(x, y);
	text.setCharacterSize(size);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	window.draw(text);
}

void drawLine(int thick ,sf::Vector2<float> start , sf::Vector2<float> end , sf::RenderWindow& window) {
	sf::Vertex line[]  = {
		sf::Vertex(start),
		sf::Vertex(end)
	};
	window.draw(line, thick, sf::Lines);
}

void drawCell(int x, int y,int diametr, sf::CircleShape & shape , sf::RenderWindow & window , sf::Font & font, std::string message) 
{
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(x, y);
	window.draw(shape);
	displayText(x + diametr/2, y + diametr/2,40, font, message, window);
}

void preOrder(Node *root)
{
	if (root == nullptr)
		return;
	
	std::cout << root->key << " ";
	preOrder(root->left);
	preOrder(root->right);
}

void traversal(Node * root)
{
	if (root == nullptr) 
		return;

	traversal(root->left);
	std::cout << root->key << std::endl;
	traversal(root->right);
}

inline std::string iToStr(int number) { return std::to_string(number); }

void printBST(Node * root , sf::RenderWindow& window , sf::Vector2<int> size ,sf::Vector2<int> place,
	sf::Font & font , sf::CircleShape &shape, int wid) 
{
	int height = 2;  // TODO calc this 

	if (root == nullptr)
		return;

	wid -= 25;
	drawCell(place.x, place.y, 25, shape, window, font, "mes");
	printBST(root->left, window, size, { place.x - wid ,size.y /height }, font, shape , wid);
	printBST(root->left, window, size, { place.x + wid ,size.y / height }, font, shape , wid);
}

int main()
{
	int dim = 25;


	sf::RenderWindow window(sf::VideoMode(640, 480), "BALANSING TREE");
	sf::CircleShape shape(dim);
	sf::Font font;
	
	if (!font.loadFromFile("font.otf")) {
		Log("Error font loading");
	}

	Node * start = new Node();
	start->key = 5;
	start->left = new Node();
	start->left->key = 5;

	start->right = new Node();
	start->right->key = 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		printBST(start, window, { 640 , 480 }, { 320 , 20 }, font, shape, 75);

		window.display();
	}

	return 0;
}
