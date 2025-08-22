#pragma once

enum class ButtonAction;

class IButtonObserver
{
public:
		virtual ~IButtonObserver() = default;
		virtual void OnButtonPressed(ButtonAction buttonActionP) = 0;
};

