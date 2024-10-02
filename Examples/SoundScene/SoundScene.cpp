#include "Melon.hpp"

using namespace Melon;

class TimeoutEventListener : EventListener
{
	void Callback(EventArgs*) override
	{
		std::cout << "time out!\n";
		audioSrc->Play();
	}
public:
	AudioSource* audioSrc;
};

int main()
{

	Window * win = Windowing::Init(800, 800, "Sounds", false);
	if (!win) return -1;

	AudioDevice* audio = Windowing::OpenAudioDevice();
	if (!audio) return -1;

	AudioBuffer buffer;
	ResourceLoader::LoadAudio(&buffer, (SourceDir+"/Examples/SoundScene/me.wav").c_str());
	
	RenderedObject2D circle = *Helpers::Objects2D::Shape(Helpers::Meshes::Circle(40));
	circle.Graphics->SetColor(Color::FromBytes(243, 28, 28, 255));
	
	Camera2D cam;
	win->MainCamera = &cam;
	
	Timer playback_timer;

	AudioSource src;
	src.Buffer = &buffer;
	src.Gain = 60;

	playback_timer.Start(2.0f);
	TimeoutEventListener lstner;
	lstner.audioSrc = &src;
	playback_timer.Timeout += (EventListener*)&lstner;

	
	for (String d : AudioDevice::GetDeviceNames())
		std::cout << d << std::endl;

	while (!win->ShouldClose())
	{
		float delta = Time::GetDelta();

		//if (playback_timer.isRunning())std::cout << Time::GetTime() << '\n';
		std::cout << ((audio->GetContextError() == 0) ? "" : "context error\n");
		std::cout << ((audio->GetDeviceError() == 0) ? "" : "device error\n");

		win->Clear(Color::FromBytes(255, 255, 255, 255), false);

		circle.Draw(win);

		win->Flip();
		Windowing::PollEvents();
	}
	src.Delete();
	buffer.Delete();
	circle.Delete();
	audio->Delete();
	Windowing::Terminate();
	return 0;
}
