#include "Melon.hpp"

class TimeoutEventListener : EventListener
{
	void Callback(EventArgs*) override
	{
		std::cout << "time out!\n";
	}
};

int main()
{

	Window * win = Windowing::Init(800, 800, "Sounds", false);
	if (!win) return -1;

	AudioDevice* audio = Windowing::OpenAudioDevice("OpenAL soft");
	if (!audio) return -1;

	AudioBuffer buffer;
	ResourceLoader::LoadAudio(&buffer, "me.wav");
	
	RenderedObject2D circle = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	circle.Graphics->SetColor(Color::FromBytes(243, 28, 28, 255));
	
	Camera2D cam;
	win->MainCamera = &cam;
	
	Timer playback_timer;

	AudioSource src;
	src.Play(&buffer);

	playback_timer.Start(2.0f);
	TimeoutEventListener lstner;
	playback_timer.Timeout += (EventListener*)&lstner;

	std::cout << audio->GetName();

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();

		if (playback_timer.isRunning())std::cout << Time::GetTime() << '\n';

		win->Clear(Color::FromBytes(255, 255, 255, 255), false);

		circle.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	src.Delete();
	buffer.Delete();
	circle.Delete();
	Windowing::Terminate();
	return 0;
}
