//This is what heppens every single frame, like character movement
scr_getinput();

if room == rm_title
{
	if global.key_jump
	{
		room_goto(rm_TEST0);
	}
}
else
{
	//MOVEMENT

	Xspeed = (global.key_right - global.key_left) * movespeed;
	Yspeed = (global.key_down - global.key_up) * movespeed;


	//COLLISIONS
	if (place_meeting(x+Xspeed,y,obj_solid))
	{
		while (!place_meeting(x+sign(Xspeed),y,obj_solid))
		{
			x = x + sign(Xspeed);
		}
		Xspeed = 0;
	}

	x += Xspeed;

	if (place_meeting(x,y+Yspeed,obj_solid))
	{
		while (!place_meeting(x,y+sign(Yspeed),obj_solid))
		{
			y = y + sign(Yspeed);
		}
		Yspeed = 0;
	}

	y += Yspeed;
	Yspeed += grv;


	if global.key_run
	{
	    if sec_spd_timer <= 0
	    {
	        movespeed = 10;
	    }
	    else
	    {
	        movespeed = 7;
	        if Xspeed != 0/* || Yspeed != 0*/
	        {
	            sec_spd_timer--;
	        }
	    }
    
	    if !global.key_left && !global.key_right/* && !global.key_up && !global.key_down*/
	    {
	        sec_spd_timer = global.runtimenum;
	        movespeed = 7;
	    }
	}
	else 
	{
	    ssp_stop--;
	    if ssp_stop <= 0
	    {
	        movespeed = 5;
	        sec_spd_timer = global.runtimenum;
	        ssp_stop = 10;
	    }
	}
	
	if (global.key_jump && !isJumping)
	{
		playerVelocityY = JUMP_FORCE; // Apply vertical force to jump
        isJumping = true; // Set jumping state to true
	}
}