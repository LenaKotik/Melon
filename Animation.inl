template <typename T>
inline void Melon::InterpolationTrack<T>::Add(Melon::Keyframe<T> kf) // TODO: rewrite this stupid shit, add some kind of sorting. push back then sort
{
	if (keyframes.Size() == 0) // no elements
	{
		keyframes.PushBack(kf);
		return;
	}
	if (keyframes.Size() == 1) // one element
	{
		if (keyframes[0].time < kf.time)
			keyframes.PushBack(kf);
		else
			keyframes.Insert(kf, 0);
		return;
	} // more than one element
	if (keyframes[0].time > kf.time) // less then first
	{
		keyframes.Insert(kf, 0);
		return;
	}
	for (int i = 0; i < keyframes.Size() - 1; i++) // inbetween
		if (keyframes[i].time < kf.time && kf.time < keyframes[i + 1].time)
		{
			keyframes.Insert(kf, i+1);
			return;
		}
	keyframes.PushBack(kf); // more than last, i guess
}
template <typename T>
inline float Melon::InterpolationTrack<T>::Length()
{
	return keyframes.PeekBack().time;
}
template <typename T>
inline T Melon::InterpolationTrack<T>::Get(float t)
{
	t = clamp(t, 0.0F, Length());
	for (int i = 0; i < keyframes.Size() - 1; i++)
	{
		Keyframe<T> kf1 = keyframes[i], kf2 = keyframes[i + 1];
		if (kf1.time <= t && t <= kf2.time)
		{
			float t_norm = (t - kf1.time) / (kf2.time - kf1.time);
			return lerp<T>(kf1.value, kf2.value, t_norm);
		}
	}
}
template <typename T>
inline void Melon::Animation<T>::Add(InterpolationTrack<T> t)
{
	tracks.PushBack(t);
}
template <typename T>
inline void Melon::Animation<T>::ComputeLength()
{
	float max_l = 0;
	for (InterpolationTrack<T> t : tracks)
	{
		float t_l = t.Length();
		if (max_l < t_l) max_l = t_l;
	}
	length = max_l;
}
template <typename T>
inline T Melon::Animation<T>::operator [](int idx)
{
	ComputeLength(); // idk
	float t = Time::GetTime();
	if (t > start_time + length && loop) start_time += length;
	t -= start_time;
	if (backward)t = length - t;
	return tracks[idx].Get(t);
}
template <typename T>
inline void Melon::Animation<T>::Play()
{
	start_time = Time::GetTime();
}
template <typename T>
inline void Melon::Animation<T>::Stop()
{
	start_time = Time::GetTime() - length;
}
template <typename T>
inline bool Melon::Animation<T>::IsPlaying()
{

}