#include "p2Log.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "UI_Image.h"
#include "UI_InputBox.h"
#include "UI_Scrollbar.h"
#include "j1Console.h"

j1Console::j1Console() : j1Module()
{
	name.create("console");
}

j1Console::~j1Console()
{

}

bool j1Console::Awake(pugi::xml_node& config)
{
	//config_file = config;

	return true;
}

bool j1Console::Start()
{
	InitConsole();
	CreateConsoleElements();

	return true;
}

bool j1Console::PreUpdate()
{	
	return true;
}

bool j1Console::Update(float dt)
{
	return true;
}

bool j1Console::PostUpdate()
{
	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

void j1Console::InitConsole()
{
	config_file.load_file("config.xml");

	console = config_file.child("config").child("console");

	// --- LOADING THE CONSOLE'S VARIABLES
	// --- BACKGROUND
	bg_position.x = console.child("bg_position").attribute("x").as_int();
	bg_position.y = console.child("bg_position").attribute("y").as_int();

	bg_rect.x = console.child("bg_rect").attribute("x").as_int();
	bg_rect.y = console.child("bg_rect").attribute("y").as_int();
	bg_rect.w = console.child("bg_rect").attribute("w").as_int();
	bg_rect.h = console.child("bg_rect").attribute("h").as_int();

	bg_colour.r = console.child("bg_colour").attribute("r").as_int();
	bg_colour.g = console.child("bg_colour").attribute("g").as_int();
	bg_colour.b = console.child("bg_colour").attribute("b").as_int();
	bg_colour.a = console.child("bg_colour").attribute("a").as_int();

	bg_isVisible = console.child("bg_isVisible").attribute("value").as_bool();
	bg_isInteractible = console.child("bg_isInteractible").attribute("value").as_bool();
	bg_isDraggable = console.child("bg_isDraggable").attribute("value").as_bool();

	// --- OUTPUT
	output_position.x = console.child("output_position").attribute("x").as_int();
	output_position.y = console.child("output_position").attribute("y").as_int();

	output_rect.x = console.child("output_rect").attribute("x").as_int();
	output_rect.y = console.child("output_rect").attribute("y").as_int();
	output_rect.w = console.child("output_rect").attribute("w").as_int();
	output_rect.h = console.child("output_rect").attribute("h").as_int();
	
	output_font_path = console.child("output_font_path").attribute("path").as_string("");
	output_font_size = console.child("output_font_size").attribute("size").as_int();

	output_font_colour.r = console.child("output_font_colour").attribute("r").as_int();
	output_font_colour.g = console.child("output_font_colour").attribute("g").as_int();
	output_font_colour.b = console.child("output_font_colour").attribute("b").as_int();
	output_font_colour.a = console.child("output_font_colour").attribute("a").as_int();

	output_isVisible = console.child("output_isVisible").attribute("value").as_bool();
	output_isInteractible = console.child("output_isInteractible").attribute("value").as_bool();;
	output_isDraggable = console.child("output_isDraggable").attribute("value").as_bool();

	// --- INPUT
	input_position.x = console.child("input_position").attribute("x").as_int();
	input_position.y = console.child("input_position").attribute("y").as_int();

	input_rect.x = console.child("input_rect").attribute("x").as_int();
	input_rect.y = console.child("input_rect").attribute("y").as_int();
	input_rect.w = console.child("input_rect").attribute("w").as_int();
	input_rect.h = console.child("input_rect").attribute("h").as_int();

	input_font_path = console.child("input_font_path").attribute("path").as_string("");
	input_font_size = console.child("input_font_size").attribute("size").as_int();

	input_font_colour.r = console.child("input_font_colour").attribute("r").as_int();
	input_font_colour.g = console.child("input_font_colour").attribute("g").as_int();
	input_font_colour.b = console.child("input_font_colour").attribute("b").as_int();
	input_font_colour.a = console.child("input_font_colour").attribute("a").as_int();

	cursor_rect;
	cursor_colour;
	input_text_offset;
	cursor_blinkFrequency;
	input_isVisible;
	input_isInteractible;
	input_isDraggable;

	// --- SCROLLBAR
	scroll_position;
	scrollbar_rect;
	thumb_rect;
	thumbOffset;
	dragArea;
	dragFactor;
	dragXAxis;
	dragYAxis;
	invertedScrolling;
	scroll_isVisible;
	scroll_isInteractible;
	scroll_isDraggable;
}

void j1Console::CreateConsoleElements()
{
	output_font = App->font->Load(output_font_path.GetString());
	input_font = App->font->Load(input_font_path.GetString());
	
	console_background = (UI_Image*)App->gui->CreateImage(UI_Element::EMPTY, 0, 0, bg_rect, bg_isVisible, bg_isInteractible, bg_isDraggable, NULL);

	console_output = (UI_Text*)App->gui->CreateText(UI_Element::TEXT, 0, 0, output_rect, output_font, output_font_colour, output_isVisible, output_isInteractible, output_isDraggable,
													console_background, NULL);

	console_input = (UI_InputBox*)App->gui->CreateInputBox(UI_Element::INPUTBOX, 10, (console_background->GetHitbox().h - input_rect.h), input_rect, input_font, input_font_colour, cursor_rect, cursor_colour, input_text_offset,
													cursor_blinkFrequency, input_isVisible, input_isInteractible, input_isDraggable, console_background, NULL);

	console_scroll = (UI_Scrollbar*)App->gui->CreateScrollbar(UI_Element::SCROLLBAR, 0, 0, scrollbar_rect, thumb_rect, thumbOffset, dragArea, dragFactor, dragXAxis, dragYAxis,
													invertedScrolling, scroll_isVisible, scroll_isDraggable, scroll_isInteractible, console_background, NULL, iPoint(0, 0));
}

Command::Command(const char* command, j1Module* callback, int min_arg, int max_arg)
{
	this->command = command;
	this->callback = callback;
	this->min_arg = min_arg;
	this->max_arg = max_arg;
}

Command* j1Console::CreateCommand(const char* command, j1Module* callback, int min_arg, int max_arg)
{
	Command* comm = nullptr;

	comm = new Command(command, callback, min_arg, max_arg);

	if (comm != nullptr)
	{
		commands.add(comm);
	}

	return comm;
}