#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
// const int PLAY_Y_HEIGHT = 450; (already defined in bubble.h)
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
const int MIDDLE_MARGIN = 250;
const int RIGHT_MARGIN = 430;

/* Bubble setup function */
vector<Bubble> create_bubbles(int level)
{
    // create initial bubbles in the game in each level and return a vector containing bubbles
    vector<Bubble> bubbles;
    if(level == 1){
        // level 1 : 2 bubbles of default radius
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    }
    else if(level == 2){
        // level 2 : 3 bubbles of double radius
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(128,0,128)));
        bubbles.push_back(Bubble(WINDOW_X/3.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(128,0,128)));
        bubbles.push_back(Bubble(2*WINDOW_X/3.0, BUBBLE_START_Y, 2*BUBBLE_DEFAULT_RADIUS, 1.5*BUBBLE_DEFAULT_VX, 0, COLOR(128,0,128)));
    }
    else if(level == 3){
        //  level 3 : 3 bubbles of quadruple radius
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(128,0,0)));
        bubbles.push_back(Bubble(WINDOW_X/3.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(128,0,0)));
        bubbles.push_back(Bubble(2*WINDOW_X/3.0, BUBBLE_START_Y, 4*BUBBLE_DEFAULT_RADIUS, 1.5*BUBBLE_DEFAULT_VX, 0, COLOR(128,0,0)));
    }
    return bubbles;
}


/* Collision checker functions */
bool is_bubble_hit(Bubble& bubble, Bullet& bullet){
    // Returns true if given bullet is intersecting with given bubble

    /* Fetch Bubble data into local vars */
    double bubbleX = bubble.get_center_x();
    double bubbleY = bubble.get_center_y();
    double bubbleR = bubble.get_radius();

    /* Fetch Bullet data into local vars */
    double bulletX = bullet.get_center_x();
    double bulletY = bullet.get_center_y();

    if( (bubbleX-bulletX)*(bubbleX-bulletX) + (bubbleY-bulletY)*(bubbleY-bulletY) < (bubbleR*bubbleR) ){
        // center of bullet is within bubble
        return 1;
    }
    else{
        return 0;
    }
}

bool is_shooter_hit(Shooter& shooter, Bubble& bubble){
    // Returns true if given bubble is intersecting with shooter else false

    /* Fetching Shooter data into local vars */
    double headX = shooter.get_head_center_x();
    double headY = shooter.get_head_center_y();
    double headR = shooter.get_head_radius();
    double bodyX = shooter.get_body_center_x();
    double bodyY = shooter.get_body_center_y();
    double body_width = shooter.get_body_width();
    double body_height = shooter.get_body_height();
    
    /* Fetching Bubble data into local vars */
    double bubbleX = bubble.get_center_x();
    double bubbleY = bubble.get_center_y();
    double bubbleR = bubble.get_radius();

    if
    (   
        // Head circle intersecting with bubble circle 
        ( (headX-bubbleX)*(headX-bubbleX) + (headY-bubbleY)*(headY-bubbleY) < (headR + bubbleR)*(headR + bubbleR) )

        || // OR

        // Body rectangle intersecting with bubble circle
        ( (bubbleX > bodyX- body_width/2 - bubbleR) && (bubbleX < bodyX+ body_width/2 + bubbleR) 
          &&
          (bubbleY > bodyY- body_height/2 - bubbleR) && (bubbleY < bodyY+ body_height/2 + bubbleR) )

    )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/* Functions to be run each frame to implement movement and collisions */
void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    // iterate through bullets vector from end to start, move each bullet and delete out of screen bullets

    for(int i = bullets.size()-1; i>=0; i--){
        if(!bullets[i].nextStep(STEP_TIME)){    // condition will evaluate to true if bullet is out of screen
            bullets.erase(bullets.begin()+i);   // hence delete bullet from vector
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles

    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void check_bullets_hit(vector<Bullet> &bullets, vector<Bubble> &bubbles, int level, int& score, Text& scoreDisplay){
    // Implements bullets and bubbles collision

    for(int i=bullets.size()-1; i >= 0; i--){       // iterate through bullets vector from end
        for(int j=bubbles.size()-1; j >= 0; j--){   // iterate through bubbles vector from end

            if( is_bubble_hit(bubbles[j], bullets[i]) ){    
                // bullet has hit bubble

                score++;                                                    // increment score
                scoreDisplay.setMessage("Score : " + to_string(score));     // update score display

                if(bubbles[j].get_radius() != BUBBLE_DEFAULT_RADIUS){
                    // if bubble is not of smallest radius then split into two bubbles of half radius
                    // with speed scaled to level and in opposite directions
                    bubbles.push_back(Bubble(bubbles[j].get_center_x(), bubbles[j].get_center_y(), bubbles[j].get_radius() /2, -level*BUBBLE_DEFAULT_VX, 0, bubbles[j].get_color()));
                    bubbles.push_back(Bubble(bubbles[j].get_center_x(), bubbles[j].get_center_y(), bubbles[j].get_radius() /2, level*BUBBLE_DEFAULT_VX, 0, bubbles[j].get_color()));
                }

                // Erase bubble and bullet which are hit
                bubbles.erase(bubbles.begin()+j);
                bullets.erase(bullets.begin()+i);
                break;
            }

        }
    }
    
}

void check_shooter_hit(Shooter& shooter, vector<Bubble> &bubbles, int& health, Text& healthDisplay){
    // Implements shooter and bubble collision

    bool change_color = 0;      // switch for color change

    for(int i=0; i < bubbles.size(); i++){  // iterate through bubbles vector

        if( is_shooter_hit(shooter, bubbles[i]) ){
            // bubble is hitting shooter
            change_color = 1;       // set switch to 1 since bubble is hitting
            if( bubbles[i].is_hitting_shooter==0 ){
                // this block runs only once as soon as bubble has hit
                health--;                                                           // decrement health 
                healthDisplay.setMessage("Health : " + to_string(health) + "/3");   // update health display
                bubbles[i].is_hitting_shooter = 1;  // set this switch to 1 so that this block doesn't run again for this bubble
            }
        }
        else{
            bubbles[i].is_hitting_shooter = 0;      // set switch back to 0 if bubble is not hitting
        }

    }

    if(change_color) shooter.set_color(COLOR(0, 0, 255));      // if any bubble is hitting change color to blue
    else             shooter.set_color( COLOR(0, 255, 0) );    // else cahnge color to green
}



int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    /* Gameplay vars */
    int level = 1;
    int score = 0;
    int health = 3;
    int time_limit = 50;            // initial time limit = 50 seconds

    int time = time_limit * 50;
    /* this variable will be decremented by 1 in each STEP_TIME(0.02s)
    hence decremented by 50 in each second hence its value is kept 50 times time_limit
    and time will be displayed by dividing this variable by 50 */

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT); // ground line
    b1.setColor(COLOR(0, 0, 255));
    
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);  // command display

    Text statusDisplay(250,250,"Level 1!");     // Text for displaying level at the end of each level and win/lose 
    statusDisplay.setColor(COLOR("blue"));
    wait(3);
    statusDisplay.hide();

    Text levelDisplay(MIDDLE_MARGIN, BOTTOM_MARGIN, "Level : 1/3");     // text for displaying level

    Text scoreDisplay(RIGHT_MARGIN, BOTTOM_MARGIN, "Score : 0");        // text for displaying score

    Text timeDisplay(LEFT_MARGIN, TOP_MARGIN, "Time : 50");             // text for displaying time

    Text healthDisplay(RIGHT_MARGIN, TOP_MARGIN, "Health : 3/3");       // text for displaying health

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(level);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {   
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);
            
            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;  
        
        }
        
        // Check and implement bullet-bubble collision
        check_bullets_hit(bullets, bubbles, level, score, scoreDisplay);

        // Check and implement shooter-bubble collision
        check_shooter_hit(shooter, bubbles, health, healthDisplay);

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // End game if health is 0 or time is 0
        if(health <= 0 || time <= 0){
            statusDisplay.setColor(COLOR("red"));
            statusDisplay.setMessage("Game Over!");
            statusDisplay.show();
            getClick();
            return 0;
        }

        // If all bubbles are destroyed then go to next level or if level 3 is cleared end game
        if(bubbles.size() == 0){
            if(level==3){
                bullets.clear();
                statusDisplay.setColor(COLOR("green"));
                statusDisplay.setMessage("Congratulations! You Win!");
                statusDisplay.show();
                getClick();
                return 0;
            }
            else{
                bullets.clear();    // clear all bullets

                // Update or reset gameplay vars
                level++;    
                health= 3;
                time_limit -= 5;            // decrement time limit by 5 for each level
                time = time_limit * 50;     
                
                statusDisplay.setMessage("Level " + to_string(level) + "!");
                statusDisplay.show();
                wait(3);
                statusDisplay.hide();

                // Update displays
                levelDisplay.setMessage("Level : " + to_string(level) + "/3");
                healthDisplay.setMessage("Health : " + to_string(health) + "/3");
                timeDisplay.setMessage("Time : " + to_string(time/50) + "/" + to_string(time_limit));

                bubbles = create_bubbles(level);
            }
        }

        time--;         // decrement time by 1 in each STEP_TIME
        timeDisplay.setMessage("Time : " + to_string(time/50) + "/" + to_string(time_limit)); // display time after dividing by 50
        wait(STEP_TIME);
    }
}
