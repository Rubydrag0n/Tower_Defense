#pragma once

class ButtonObject
{
public:
	ButtonObject();
	virtual ~ButtonObject();

	//is called when the button is pressed. Button_ID is from the button to note which button was pressed
	virtual void on_button_press(int Button_ID) = 0;
private:

};