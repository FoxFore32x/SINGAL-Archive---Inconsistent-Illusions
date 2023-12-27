// Los recursos de Script han cambiado para la v2.3.0 Consulta
// https://help.yoyogames.com/hc/en-us/articles/360005277377 para más información
function scr_getinput(){
	gamepad_set_axis_deadzone(0,0.27);
	gamepad_set_axis_deadzone(global.nxgp,0.27);

	global.key_left = keyboard_check(ord("A")) || (gamepad_axis_value(0, gp_axislh) < 0) || (gamepad_button_check(0, gp_padl)) || (gamepad_axis_value(global.nxgp, gp_axislh) < 0) || (gamepad_button_check(global.nxgp, gp_padl));
	global.key_right = keyboard_check(ord("D")) || (gamepad_axis_value(0, gp_axislh) > 0) || (gamepad_button_check(0, gp_padr)) || (gamepad_axis_value(global.nxgp, gp_axislh) > 0) || (gamepad_button_check(global.nxgp, gp_padr));
	global.key_up = keyboard_check(ord("W")) || (gamepad_axis_value(0, gp_axislv) < 0) || (gamepad_button_check(0, gp_padu))|| (gamepad_axis_value(global.nxgp, gp_axislv) < 0) || (gamepad_button_check(global.nxgp, gp_padu));
	global.key_down = keyboard_check(ord("S")) || (gamepad_axis_value(0, gp_axislv) > 0) || (gamepad_button_check(0, gp_padd)) || (gamepad_axis_value(global.nxgp, gp_axislv) > 0) || (gamepad_button_check(global.nxgp, gp_padd));

	//ACTIONS
	global.key_run = keyboard_check(ord("O")) || (gamepad_button_check(0, gp_face3)) || (gamepad_button_check(global.nxgp, gp_face3));
	global.key_jump = keyboard_check(ord("P")) || gamepad_button_check(0, gp_face1) || (gamepad_button_check(global.nxgp, gp_face2));
	global.key_crouch = keyboard_check(ord("I")) || (gamepad_button_check(0, gp_face2)) || (gamepad_button_check(0, gp_shoulderrb))|| (gamepad_button_check(global.nxgp, gp_shoulderrb));
}