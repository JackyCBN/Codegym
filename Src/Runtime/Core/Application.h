#pragma once
#include "Base.h"

namespace codegym::runtime {
    class Window;
    struct WindowDesc;
    class Event;


    class CG_API Application
    {

    public:
        Application();
        virtual ~Application();

        void Run();
        bool OnFrame();

        virtual void OnQuit();
        virtual void Init();
        virtual void OnEvent(Event& e);
        virtual void OnRender();
        virtual void OnUpdate();
        

        Window* GetWindow() const
        {
            return m_Window.get();
        }
        
        

        static Application& Get()
        {
            return *s_Instance;
        }

        static void Release()
        {
            if(s_Instance)
                delete s_Instance;
            s_Instance = nullptr;
        }

    private:
        
        std::unique_ptr<Window> m_Window;
        
        static Application* s_Instance;
        

       // NONCOPYABLE(Application)
    };

    //Defined by client
    Application* CreateApplication();
}
