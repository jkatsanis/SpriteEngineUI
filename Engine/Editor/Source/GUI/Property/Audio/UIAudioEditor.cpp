#include "UIAudioEditor.h"

#include <iostream>

//Constructor

void spe::UIAudioEditor::Init()
{
}

spe::UIAudioEditor::UIAudioEditor()
{
	this->Display = false;
	this->m_CursorSpace = 30;
	this->m_SelectedAudioType = 0;
	this->m_IsPlaying = false;
	memset(this->m_AudioNameBuffer, 0, sizeof(this->m_AudioNameBuffer));
	memset(this->m_TempAudioPath, 0, sizeof(this->m_TempAudioPath));
	this->m_TempVolume = 100.0f;
	this->m_TempPitch = 1.0f;
	this->m_TempPlayOnStart = false;
	this->m_TempLoop = false;

	// Sound-specific defaults
	this->m_TempRelativeToListener = true;
	this->m_TempMinDistance = 1.0f;
	this->m_TempAttenuation = 1.0f;

	// Music-specific defaults
	this->m_TempLoopPointOffset = 0.0f;
	this->m_TempLoopPointLength = 0.0f;
}

// Public methods

void spe::UIAudioEditor::Render()
{
	if (!this->m_ptr_GUIRepo->AudioData.IsOpen || this->m_ptr_GUIRepo->InspectorSprite == nullptr || this->m_ptr_GUIRepo->ptr_CurrentAudio == nullptr)
	{
		this->Hovered = false;
		return;
	}
	// this->m_BackgroundCounter = START_CNT_BG;
	ImGui::Begin("##Audio", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	this->m_ptr_GUIRepo->AudioData.IsOpen = spe::UIUtility::RenderCloseRectangle(
		50, ICON_FA_FILE_CODE, "##close-rectangle-audio", "Audio", 0);

	if (!this->m_ptr_GUIRepo->AudioData.IsOpen && this->m_TempAudioPath[0] == '\0'){
		// remove audio
		this->m_ptr_GUIRepo->InspectorSprite->Audio.RemoveAudio(this->m_ptr_GUIRepo->ptr_CurrentAudio->GetName());
	}

	if (this->m_ptr_GUIRepo->AudioData.Reload)
	{
		this->m_ptr_GUIRepo->AudioData.Reload = false;
		spe::UIUtility::SetWindowScreenMiddle(WINDOW_SIZE_AUDIO_EDITOR);

		LoadSettingsFromAudio();

		// Reset playing state
		this->m_IsPlaying = false;
	}

	// Audio Name Input
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::Text("Name:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##audioName", this->m_AudioNameBuffer, sizeof(this->m_AudioNameBuffer));

	// Audio Type Dropdown
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	ImGui::Text("Type:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	const char* audioTypes[] = { "SOUND", "MUSIC" };
	ImGui::Combo("##audioType", &this->m_SelectedAudioType, audioTypes, IM_ARRAYSIZE(audioTypes));
	ImGui::Text("Source File:");
	ImGui::SameLine();
	ImGui::InputText("##audioSourceFile", this->m_TempAudioPath, sizeof(this->m_TempAudioPath));

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->DragAndDropPath != " ")
	{
		strncpy(this->m_TempAudioPath, this->m_ptr_GUIRepo->DragAndDropPath.c_str(), sizeof(this->m_TempAudioPath) - 1);
		this->m_TempAudioPath[sizeof(this->m_TempAudioPath) - 1] = '\0';
	}


	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("General Options");
	ImGui::Spacing();

	// Volume Slider
	ImGui::Text("Volume");
	ImGui::SameLine();
	ImGui::SliderFloat("##volumeAudio", &this->m_TempVolume, 0.0f, 100.0f);

	// Pitch Control (Slider + Manual Input)
	ImGui::Text("Pitch");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(300);
	ImGui::SliderFloat("##pitchAudioSlider", &this->m_TempPitch, 0.1f, 3.0f);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(80);
	if (ImGui::InputFloat("##pitchAudioInput", &this->m_TempPitch, 0.0f, 0.0f, "%.2f"))
	{
		// Clamp to reasonable values to prevent issues
		if (this->m_TempPitch < 0.01f) {
			this->m_TempPitch = 0.01f;
		}
	}

	ImGui::Text("Play On Start");
	ImGui::SameLine();
	ImGui::Checkbox("##onstartAudio", &this->m_TempPlayOnStart);

	ImGui::Text("Loop");
	ImGui::SameLine();
	ImGui::Checkbox("##loopAudio", &this->m_TempLoop);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Sound Options");
	ImGui::Spacing();

	// Show sound-specific options only if type is SOUND
	if (this->m_SelectedAudioType == 0) // SOUND
	{
		ImGui::Text("Relative to Listener");
		ImGui::SameLine();
		ImGui::Checkbox("##relativeToListener", &this->m_TempRelativeToListener);

		// Grey out position if relative to listener is true
		if (this->m_TempRelativeToListener)
		{
			ImGui::BeginDisabled();
		}

		// ImGui::Text("Position X");
		// ImGui::SameLine();
		// ImGui::SetNextItemWidth(150);
		// ImGui::InputFloat("##positionX", &this->m_TempPositionX, 0.0f, 0.0f, "%.2f");
		//
		// ImGui::Text("Position Y");
		// ImGui::SameLine();
		// ImGui::SetNextItemWidth(150);
		// ImGui::InputFloat("##positionY", &this->m_TempPositionY, 0.0f, 0.0f, "%.2f");

		ImGui::Text("Min Distance");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::InputFloat("##minDistance", &this->m_TempMinDistance, 0.0f, 0.0f, "%.2f");
		if (this->m_TempMinDistance < 0.0f) this->m_TempMinDistance = 0.0f;

		ImGui::Text("Attenuation");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::InputFloat("##attenuation", &this->m_TempAttenuation, 0.0f, 0.0f, "%.2f");
		if (this->m_TempAttenuation < 0.0f) this->m_TempAttenuation = 0.0f;

		if (this->m_TempRelativeToListener)
		{
			ImGui::EndDisabled();
		}
	}
	else
	{
		ImGui::TextDisabled("Sound options are only available for SOUND type");
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Music Options");
	ImGui::Spacing();

	// Show music-specific options only if type is MUSIC
	if (this->m_SelectedAudioType == 1) // MUSIC
	{
		// Grey out loop points if loop is disabled
		if (!this->m_TempLoop)
		{
			ImGui::BeginDisabled();
		}

		ImGui::Text("Loop Point Offset (s)");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::InputFloat("##loopPointOffset", &this->m_TempLoopPointOffset, 0.0f, 0.0f, "%.2f");
		if (this->m_TempLoopPointOffset < 0.0f) this->m_TempLoopPointOffset = 0.0f;

		ImGui::Text("Loop Point Length (s)");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::InputFloat("##loopPointLength", &this->m_TempLoopPointLength, 0.0f, 0.0f, "%.2f");
		if (this->m_TempLoopPointLength < 0.0f) this->m_TempLoopPointLength = 0.0f;

		if (!this->m_TempLoop)
		{
			ImGui::EndDisabled();
		}
	}
	else
	{
		ImGui::TextDisabled("Music options are only available for MUSIC type");
	}


	if (Style::DisplaySymbolButtonWithText(ICON_FA_SAVE, "Save"))
	{
		ApplySettingsToAudio();
		this->m_ptr_GUIRepo->AudioData.IsOpen = false;
	}
	if (Style::DisplaySymbolButtonWithText(ICON_FA_UNDO, "Reset")) {
		LoadSettingsFromAudio();
	}
	ImGui::SetWindowSize(WINDOW_SIZE_AUDIO_EDITOR);

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

	if (!this->Hovered)
		this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImGui::GetWindowSize());
	ImGui::End();
}

void spe::UIAudioEditor::ResetAudio()
{
	this->Display = false;
	this->m_IsPlaying = false;
	memset(this->m_AudioNameBuffer, 0, sizeof(this->m_AudioNameBuffer));
	memset(this->m_TempAudioPath, 0, sizeof(this->m_TempAudioPath));
	this->m_SelectedAudioType = 0;
	this->m_TempVolume = 100.0f;
	this->m_TempPitch = 1.0f;
	this->m_TempPlayOnStart = false;
	this->m_TempLoop = false;

	// Sound-specific defaults
	this->m_TempRelativeToListener = true;
	this->m_TempPositionX = 0.0f;
	this->m_TempPositionY = 0.0f;
	this->m_TempMinDistance = 1.0f;
	this->m_TempAttenuation = 1.0f;

	// Music-specific defaults
	this->m_TempLoopPointOffset = 0.0f;
	this->m_TempLoopPointLength = 0.0f;
}

void spe::UIAudioEditor::LoadSettingsFromAudio()
{

	// Load name
	const std::string currentName = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetName();
	strncpy(this->m_AudioNameBuffer, currentName.c_str(), sizeof(this->m_AudioNameBuffer) - 1);
	this->m_AudioNameBuffer[sizeof(this->m_AudioNameBuffer) - 1] = '\0';

	// Load type
	this->m_SelectedAudioType = static_cast<int>(this->m_ptr_GUIRepo->ptr_CurrentAudio->GetType());

	// Load path
	const std::string currentPath = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetPath();
	strncpy(this->m_TempAudioPath, currentPath.c_str(), sizeof(this->m_TempAudioPath) - 1);
	this->m_TempAudioPath[sizeof(this->m_TempAudioPath) - 1] = '\0';

	// Load settings
	this->m_TempVolume = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetVolume();
	this->m_TempPitch = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetPitch();
	this->m_TempPlayOnStart = this->m_ptr_GUIRepo->ptr_CurrentAudio->PlayOnStart;
	this->m_TempLoop = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetLoop();

	// Load sound-specific settings
	if (this->m_ptr_GUIRepo->ptr_CurrentAudio->GetType() == spe::Audio::AudioType::SOUND)
	{
		const sf::Sound* soundObj = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetSoundObject();
		this->m_TempRelativeToListener = soundObj->isRelativeToListener();
		const sf::Vector3f pos = soundObj->getPosition();
		this->m_TempPositionX = pos.x;
		this->m_TempPositionY = pos.y;
		this->m_TempMinDistance = soundObj->getMinDistance();
		this->m_TempAttenuation = soundObj->getAttenuation();
	}

	// Load music-specific settings
	if (this->m_ptr_GUIRepo->ptr_CurrentAudio->GetType() == spe::Audio::AudioType::MUSIC)
	{
		const sf::Music* musicObj = this->m_ptr_GUIRepo->ptr_CurrentAudio->GetMusicObject();
		const sf::Music::TimeSpan loopPoints = musicObj->getLoopPoints();
		this->m_TempLoopPointOffset = loopPoints.offset.asSeconds();
		this->m_TempLoopPointLength = loopPoints.length.asSeconds();
	}
}

void spe::UIAudioEditor::ApplySettingsToAudio()
{
	// if path is empty
	if (this->m_TempAudioPath[0] == '\0') {
		this->m_ptr_GUIRepo->InspectorSprite->Audio.RemoveAudio(this->m_ptr_GUIRepo->ptr_CurrentAudio->GetName());
		return;
	}

	const std::string oldName(this->m_AudioNameBuffer);
	std::string newName = oldName;
	int counter = 0;
	const auto* found = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAudioByName(newName);
	while (found != nullptr && found != this->m_ptr_GUIRepo->ptr_CurrentAudio) {
		newName = oldName + " (" + std::to_string(++counter) + ")";
		found = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAudioByName(newName);
	}

	// if path or type is changed reconfigure audio
	if (this->m_ptr_GUIRepo->ptr_CurrentAudio->GetPath() != std::string(this->m_TempAudioPath) ||
	    this->m_ptr_GUIRepo->ptr_CurrentAudio->GetType() != static_cast<spe::Audio::AudioType>(this->m_SelectedAudioType))
	{
		this->m_ptr_GUIRepo->ptr_CurrentAudio->Configure(std::string(this->m_TempAudioPath),
		    static_cast<spe::Audio::AudioType>(this->m_SelectedAudioType),
		    newName);
	}
	else {
		this->m_ptr_GUIRepo->ptr_CurrentAudio->SetName(newName);
	}

	// Apply settings
	this->m_ptr_GUIRepo->ptr_CurrentAudio->SetVolume(this->m_TempVolume);
	this->m_ptr_GUIRepo->ptr_CurrentAudio->SetPitch(this->m_TempPitch);
	this->m_ptr_GUIRepo->ptr_CurrentAudio->PlayOnStart = this->m_TempPlayOnStart;
	this->m_ptr_GUIRepo->ptr_CurrentAudio->SetLoop(this->m_TempLoop);

	// Apply sound-specific settings
	if (this->m_ptr_GUIRepo->ptr_CurrentAudio->GetType() == spe::Audio::AudioType::SOUND)
	{
		this->m_ptr_GUIRepo->ptr_CurrentAudio->SetRelativeToListener(this->m_TempRelativeToListener);
		this->m_ptr_GUIRepo->ptr_CurrentAudio->SetPosition(this->m_TempPositionX, this->m_TempPositionY);
		this->m_ptr_GUIRepo->ptr_CurrentAudio->SetMinDistance(this->m_TempMinDistance);
		this->m_ptr_GUIRepo->ptr_CurrentAudio->SetAttenuation(this->m_TempAttenuation);
	}

	// Apply music-specific settings
	if (this->m_ptr_GUIRepo->ptr_CurrentAudio->GetType() == spe::Audio::AudioType::MUSIC)
	{
		if (this->m_TempLoop &&
		    this->m_TempLoopPointLength > 0.0f &&
		    this->m_TempLoopPointOffset > 0.0f)
		{
			this->m_ptr_GUIRepo->ptr_CurrentAudio->SetLoopPoints(this->m_TempLoopPointOffset, this->m_TempLoopPointLength);
		}
	}
}
