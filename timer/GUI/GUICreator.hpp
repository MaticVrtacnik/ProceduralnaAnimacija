#ifndef GUI_CREATOR_HPP
#define GUI_CREATOR_HPP

namespace Engine{
	namespace GUI{

		class GUIBase;

		void createGUI(int screenWidth, int screenHeight, GUIBase *index);
		void createEditorGUI(int screenWidth, int screenHeight, GUIBase *index);

	}
}


#endif //GUI_CREATOR_HPP