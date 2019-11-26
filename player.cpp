#include"player.h"
#include"actor.h"
#include"stack.h"
#include"list.h"
#include"point.h"
#include"aquarium.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Actor and its member variables
//		Remembers and discovers the starting point.
Player::Player(Aquarium* aquarium, Point p, std::string name, char sprite)
	:Actor(aquarium, p, name, sprite),
	m_look(),
	m_discovered(),
	m_btQueue(),
	m_btStack(),
	m_toggleBackTracking(false) {

	// Discover the starting point
	m_discovered.push_front(p);
	m_look.push(p);
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Actor::getState() == State::STUCK;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Actor::getState() == State::FREEDOM;
}
//  toggleBackTrack( ... )
//		Turn on/off backtracking
void Player::toggleBackTrack(bool toggle) {
	m_toggleBackTracking = toggle;
}

//  getTargetPoint()
//		Get the point the player wants to look around next.
//      If m_look is empty then return an invalid point.
Point Player::getTargetPoint() const {
	if (m_look.empty()) return Point(-1, -1);
	return m_look.peek();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Point& p) const {
	return (m_discovered.find(p) != -1);
}



// say()
//		What does the player say?
//		Already Implemented, nothing to do here
void Player::say() {

	// Freedom supercedes being eaten
	if (getState() == State::FREEDOM) {
		std::cout << getName() << ": WEEEEEEEEE!";
		return;
	}

	// Being eaten supercedes being lost
	switch (getInteract()) {
	case Interact::ATTACK:
		std::cout << getName() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (getState()) {
		case State::LOOKING:
			std::cout << getName() << ": Where is the exit?";
			break;
		case State::STUCK:
			std::cout << getName() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACK:
			std::cout << getName() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOKING). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired point to look around is not adjacent (more than one cell away), 
//		at this point the player must backtrack (State::BACKTRACK) to a point 
//		that is adjacent to the next point the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or 
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement 
//		of the player.  
//
//		Backtracking is challenging, save it for the very very very last thing.
//		Make sure the STATE::LOOKING aspect compiles and works first.
void Player::update() {
	if (m_toggleBackTracking == false) {
		Point curr = getTargetPoint();
		int rows = getAquarium()->rows();
		int cols = getAquarium()->cols();
		int x = curr.getX();
		int y = curr.getY();

		m_look.print();

		// if stack is empty - no solution
		if (m_look.empty() == true) {
			cout << "i should be stuck";
			setState(State::STUCK);
		}

		//pop the top point off the stack
		if (m_look.empty() == false) {
			m_look.pop();
		}
		else {
			return;
		}
		//if we are at endpoint, we are done
		if (getAquarium()->getEndPoint() == curr) {
			setState(State::FREEDOM);
		}

		setPosition(curr);

		// check west
		curr.set(x - 1, y);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
			curr.set(x + 1, y);
		}
		// check east
		curr.set(x + 1, y);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
			curr.set(x - 1, y);
		}
		// check north
		curr.set(x, y - 1);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
			curr.set(x, y + 1);
		}
		// check south
		curr.set(x, y + 1);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
			curr.set(x, y - 1);
		}
		cout << "stack: ";
		m_look.print();
		cout << endl;
		cout << "list: ";
		m_discovered.print();

		cout << "end of first: ";
	}
	else {
		Point curr = getTargetPoint();
		int rows = getAquarium()->rows();
		int cols = getAquarium()->cols();
		int x = curr.getX();
		int y = curr.getY();

		cout << "stack before pop: ";
		m_look.print();
		cout << "curr before set: " << curr << endl;
		

		// if stack is empty - no solution
		if (m_look.empty() == true) {
			cout << "i should be stuck";
			setState(State::STUCK);
		}

		//pop the top point off the stack
		if (m_look.empty() == false) {
			m_look.pop();
		}
		else {
			return;
		}
		//if we are at endpoint, we are done
		if (getAquarium()->getEndPoint() == curr) {
			setState(State::FREEDOM);
		}



		setPosition(curr);
		cout << "curr after set: " << curr << endl;
		if (m_btStack.empty() == true) {
			m_btStack.push(curr);
		}





		// check west
		curr.set(x - 1, y);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
		}
		else if (getAquarium()->isCellOpen(curr) && discovered(curr) == true) {
			m_btStack.push(curr);
		}
		curr.set(x + 1, y);

		// check east
		curr.set(x + 1, y);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
		}
		else if (getAquarium()->isCellOpen(curr) && discovered(curr) == true) {
			m_btQueue.push(curr);
		}
		curr.set(x - 1, y);

		// check north
		curr.set(x, y - 1);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
		}
		else if (getAquarium()->isCellOpen(curr) && discovered(curr) == true) {
			m_btStack.push(curr);
		}
		curr.set(x, y + 1);
		

		cout << "curr before south" << curr;
		// check south
		curr.set(x, y + 1);
		if (getAquarium()->isCellOpen(curr) && discovered(curr) == false) {
			m_discovered.push_front(curr);
			m_look.push(curr);
		}
		else if (getAquarium()->isCellOpen(curr) && discovered(curr) == true) {
			m_btStack.push(curr);
		}
		curr.set(x, y - 1);
		cout << "curr right after check south" << curr;
		cout << "btStack after south";
		m_btStack.print();


		curr = m_look.peek();
		cout << "btstack before backtrack: ";
		m_btStack.print();
		cout << "curr before backtrack: " << curr << endl;
		curr = m_btStack.peek();

		if (getAquarium()->isCellOpen(curr) == true && discovered(curr) == true) {
			cout << "pop bstack and push stack" << endl;
				m_look.push(m_btStack.peek());
				m_btStack.pop();
		}
		cout << "curr after backtrack: " << curr << endl;
		//cout << endl;
		//cout << "curr after directions: " << curr << endl;


		cout << "stack after backtrack: ";
		m_look.print();
		cout << endl;

		cout << "btstack after backtrack: ";
		m_btStack.print();


		cout << "end of first: ";
	}
}

	//// Set by the settings file and checked here
	//// if(m_toggleBackTracking) { ... code relating to backtracking 
