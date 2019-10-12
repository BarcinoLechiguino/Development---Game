#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_load = want_to_save = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();

	config_file.reset();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	bool ret = LoadConfig();

	// self-config
	title.create(app_config.child("title").child_value());
	organization.create(app_config.child("organization").child_value());

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}


// ---------------------------------------------
bool j1App::LoadConfig()
{
	bool ret = true;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = config_file.child("config");
		app_config = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	// TODO 2: This is a good place to call load / Save functions
	if (want_to_load == true)
	{
		LoadGameNow();
	}

	if (want_to_save == true)
	{
		SaveGameNow();
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

void j1App::Load_Game()
{
	want_to_load = true;
}

void j1App::Save_Game() const
{
	want_to_save = true;
}

// TODO 5: Fill the application load function
// Start by opening the file as an xml_document (as with config file)
bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data; //Creates the tree and consists of loading functions.
	pugi::xml_node root;  //Handle node that can point to any node inside the document, including the document itself. Allows for document inspection and modification. 

	//load_file() destroys the existing tree and tries to load a new one from the specified file.
	pugi::xml_parse_result result = data.load_file("save.xml"); //parse_result recieves the result of the load_file operation. Acts as a bool.

	if (result != NULL )
	{
		LOG("Loading new save from save.xml");

		root = data.child("save"); //Sets the handle pointer to the save node.

		//Creates a list and sets the initial pointer to  the first module.
		p2List_item<j1Module*>* item = modules.start; 
		ret = true;

		//Cycles as long as the pointer of the list isn't NULL (and ret).
		while (item != NULL && ret == true)
		{
			//It iterates all modules and calls their load methods.
			ret = item->data->Load(root.child(item->data->name.GetString())); //Sends the xml section as an argument. 
			item = item->next;
		}

		data.reset();

		if (ret == true) { LOG("... finished loading"); }
		else { LOG("... loading process interrupted with error on module %s", item->data->name.GetString()); }
	}

	else
	{
		LOG("Could not parse game save xml file %s. Pugi error %s", load_game.GetString(), result.description());
	}

	want_to_load = false;
	return ret;
}

// TODO 7: Fill the application save function
// Generate a new pugi::xml_document and create a node for each module.
// Call each module's save function and then save the file using pugi::xml_document::save_file()
bool j1App::SaveGameNow()  const
{
	bool ret = false;
	LOG("Saving save to save.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	//append_ adds the node with the save name.
	root = data.append_child("save");

	p2List_item<j1Module*>* item = modules.start;
	ret = true;

	while (item != NULL && ret == true)
	{
		//append_child adds the node with the given text.
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if (ret == true) 
	{ 
		//save_file() saves the whole document to a file and returns true on success.
		data.save_file("save.xml");
		LOG("... finished saving.");
	}
	else
	{
		LOG("Save process halted by an error in the module %s", item->data->name.GetString());
	}

	data.reset();
	want_to_save = false;
	return ret;
}
