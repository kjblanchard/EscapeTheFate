# Escape The Fate

### Leaks

==10882==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 352 byte(s) in 11 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #6 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #7 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #8 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #9 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #10 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #11 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #12 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #13 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #14 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #15 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #16 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #17 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #18 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #19 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #20 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #21 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #22 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #23 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #24 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #25 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #26 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #27 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #28 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #29 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204

Direct leak of 128 byte(s) in 4 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x5580506810e8 in Etf::UINineSlice::UINineSlice(Etf::UINineSliceArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiNineSlice.cpp:24
    #4 0x558050666425 in createNineSliceObject /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:139
    #5 0x558050666f9b in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:171
    #6 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #7 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #8 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #9 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #10 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #11 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #12 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #13 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #14 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #15 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #16 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #17 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #18 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #19 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #20 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #21 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #22 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #23 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #24 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #25 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121
    #26 0x5580506aa3c7 in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:201
    #27 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #28 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #29 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133

Direct leak of 96 byte(s) in 3 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x55805094f7a2 in createBackgroundsFromTilemap /home/kevin/git/EscapeTheFate/SupergoonEngine/src/map.c:306
    #4 0x558050951bc1 in loadMapInternal /home/kevin/git/EscapeTheFate/SupergoonEngine/src/map.c:461
    #5 0x558050951d20 in LoadMapFromBuffer /home/kevin/git/EscapeTheFate/SupergoonEngine/src/map.c:479
    #6 0x5580506a9e41 in Etf::Engine::loadSceneInternal() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:150
    #7 0x5580506aa37c in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:191
    #8 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #9 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #10 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #11 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #12 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #13 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #14 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #15 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #16 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Direct leak of 96 byte(s) in 3 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x5580506810e8 in Etf::UINineSlice::UINineSlice(Etf::UINineSliceArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiNineSlice.cpp:24
    #4 0x558050666425 in createNineSliceObject /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:139
    #5 0x558050666f9b in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:171
    #6 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #7 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #8 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #9 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #10 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #11 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #12 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #13 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #14 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #15 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #16 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #17 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #18 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #19 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121
    #20 0x5580506aa3c7 in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:201
    #21 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #22 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #23 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #24 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #25 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #26 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #27 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #28 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #29 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Direct leak of 64 byte(s) in 2 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #6 0x558050654f5a in Etf::EnemyBattler::EnemyBattler(Etf::BattlerArgs const&) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/EnemyBattler.cpp:12
    #7 0x55805069191a in loadEnemies /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:148
    #8 0x55805069237a in loadBattle /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:188
    #9 0x558050692400 in triggerStateChange /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:204
    #10 0x5580506924e6 in Etf::BattleSystem::BattleSystemUpdate() /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:235
    #11 0x558050601d6e in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:98
    #12 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #13 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #14 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #15 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #16 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #17 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #18 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #19 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Direct leak of 64 byte(s) in 2 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #6 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #7 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #8 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #9 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #10 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #11 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #12 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #13 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #14 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #15 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #16 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #17 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #18 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #19 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #20 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #21 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #22 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #23 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #24 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #25 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #26 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #27 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #28 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #29 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121

Direct leak of 64 byte(s) in 2 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #6 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #7 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #8 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #9 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #10 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #11 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #12 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #13 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #14 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #15 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #16 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #17 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #18 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #19 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #20 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #21 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #22 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #23 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #24 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #25 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #26 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #27 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #28 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #29 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #6 0x5580506875c8 in updateDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:53
    #7 0x558050687a58 in startDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:88
    #8 0x558050687fc0 in progressCurrentInteraction /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:129
    #9 0x5580506885f9 in Etf::DialogSystem::TextBoxInteractionUpdate(Etf::Textbox*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:195
    #10 0x558050655d7c in Etf::Textbox::Interact() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Textbox.cpp:38
    #11 0x55805060af23 in Etf::LocalPlayer::handleInteractions() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:145
    #12 0x55805060a113 in Etf::LocalPlayer::Update() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:90
    #13 0x55805069f544 in Etf::GameObjectSystem::Update() /home/kevin/git/EscapeTheFate/src/systems/GameObjectSystem.cpp:55
    #14 0x558050601d1d in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:94
    #15 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #16 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #17 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #18 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #19 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #20 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #21 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #22 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #6 0x5580506629a6 in Etf::BattlerUI::UpdateHP(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/battle/battlerUI.cpp:95
    #7 0x55805064e7d3 in Etf::PlayerBattler::PlayerBattler(Etf::BattlerArgs const&) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:20
    #8 0x55805069148e in loadPlayers /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:130
    #9 0x558050692375 in loadBattle /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:187
    #10 0x558050692400 in triggerStateChange /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:204
    #11 0x5580506924e6 in Etf::BattleSystem::BattleSystemUpdate() /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:235
    #12 0x558050601d6e in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:98
    #13 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #14 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #15 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #16 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #17 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #18 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #19 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #20 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #6 0x5580506875c8 in updateDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:53
    #7 0x558050687a58 in startDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:88
    #8 0x558050687d21 in startNewDialogInteraction /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:107
    #9 0x5580506885eb in Etf::DialogSystem::TextBoxInteractionUpdate(Etf::Textbox*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:187
    #10 0x558050655d7c in Etf::Textbox::Interact() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Textbox.cpp:38
    #11 0x55805060af23 in Etf::LocalPlayer::handleInteractions() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:145
    #12 0x55805060a113 in Etf::LocalPlayer::Update() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:90
    #13 0x55805069f544 in Etf::GameObjectSystem::Update() /home/kevin/git/EscapeTheFate/src/systems/GameObjectSystem.cpp:55
    #14 0x558050601d1d in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:94
    #15 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #16 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #17 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #18 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #19 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #20 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #21 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #22 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #6 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #7 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #8 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #9 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #10 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #11 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #12 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #13 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #14 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #15 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #16 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #17 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #18 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #19 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #20 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #21 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #22 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #23 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121
    #24 0x5580506aa3c7 in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:201
    #25 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #26 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #27 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #28 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #29 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x558050963533 in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:116
    #2 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #3 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #4 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #5 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #6 0x5580506552bd in Etf::EnemyBattler::takeDamageImpl(int) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/EnemyBattler.cpp:24
    #7 0x558050611d39 in Etf::Battler::TakeDamage(int) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Battler.cpp:37
    #8 0x558050650278 in Etf::PlayerBattler::handleInputTargetSelection() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:167
    #9 0x5580506505d6 in Etf::PlayerBattler::handleInput() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:190
    #10 0x55805064f4ed in Etf::PlayerBattler::updateImpl() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:113
    #11 0x5580506125c5 in Etf::Battler::Update() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Battler.hpp:28
    #12 0x55805069f544 in Etf::GameObjectSystem::Update() /home/kevin/git/EscapeTheFate/src/systems/GameObjectSystem.cpp:55
    #13 0x558050601d1d in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:94
    #14 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #15 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #16 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #17 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #18 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #19 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #20 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #21 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 416 byte(s) in 13 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #9 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #10 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #11 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #12 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #13 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #14 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #15 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #16 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #17 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #18 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #19 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #20 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #21 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #22 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #23 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #24 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #25 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #26 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #27 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #28 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #29 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103

Indirect leak of 129 byte(s) in 4 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x5580506810e8 in Etf::UINineSlice::UINineSlice(Etf::UINineSliceArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiNineSlice.cpp:24
    #7 0x558050666425 in createNineSliceObject /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:139
    #8 0x558050666f9b in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:171
    #9 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #10 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #11 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #12 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #13 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #14 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #15 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #16 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #17 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #18 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #19 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #20 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #21 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #22 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #23 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #24 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #25 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #26 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #27 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #28 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121
    #29 0x5580506aa3c7 in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:201

Indirect leak of 102 byte(s) in 3 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x55805094f7a2 in createBackgroundsFromTilemap /home/kevin/git/EscapeTheFate/SupergoonEngine/src/map.c:306
    #7 0x558050951bc1 in loadMapInternal /home/kevin/git/EscapeTheFate/SupergoonEngine/src/map.c:461
    #8 0x558050951d20 in LoadMapFromBuffer /home/kevin/git/EscapeTheFate/SupergoonEngine/src/map.c:479
    #9 0x5580506a9e41 in Etf::Engine::loadSceneInternal() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:150
    #10 0x5580506aa37c in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:191
    #11 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #12 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #13 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #14 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #15 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #16 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #17 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #18 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #19 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 99 byte(s) in 3 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x5580506810e8 in Etf::UINineSlice::UINineSlice(Etf::UINineSliceArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiNineSlice.cpp:24
    #7 0x558050666425 in createNineSliceObject /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:139
    #8 0x558050666f9b in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:171
    #9 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #10 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #11 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #12 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #13 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #14 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #15 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #16 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #17 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #18 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #19 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #20 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #21 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #22 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121
    #23 0x5580506aa3c7 in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:201
    #24 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #25 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #26 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #27 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #28 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #29 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137

Indirect leak of 66 byte(s) in 2 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #9 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #10 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #11 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #12 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #13 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #14 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #15 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #16 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #17 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #18 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #19 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #20 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #21 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #22 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #23 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #24 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #25 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #26 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #27 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #28 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #29 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103

Indirect leak of 64 byte(s) in 2 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #9 0x558050654f5a in Etf::EnemyBattler::EnemyBattler(Etf::BattlerArgs const&) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/EnemyBattler.cpp:12
    #10 0x55805069191a in loadEnemies /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:148
    #11 0x55805069237a in loadBattle /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:188
    #12 0x558050692400 in triggerStateChange /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:204
    #13 0x5580506924e6 in Etf::BattleSystem::BattleSystemUpdate() /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:235
    #14 0x558050601d6e in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:98
    #15 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #16 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #17 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #18 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #19 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #20 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #21 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #22 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 33 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #9 0x5580506875c8 in updateDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:53
    #10 0x558050687a58 in startDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:88
    #11 0x558050687fc0 in progressCurrentInteraction /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:129
    #12 0x5580506885f9 in Etf::DialogSystem::TextBoxInteractionUpdate(Etf::Textbox*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:195
    #13 0x558050655d7c in Etf::Textbox::Interact() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Textbox.cpp:38
    #14 0x55805060af23 in Etf::LocalPlayer::handleInteractions() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:145
    #15 0x55805060a113 in Etf::LocalPlayer::Update() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:90
    #16 0x55805069f544 in Etf::GameObjectSystem::Update() /home/kevin/git/EscapeTheFate/src/systems/GameObjectSystem.cpp:55
    #17 0x558050601d1d in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:94
    #18 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #19 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #20 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #21 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #22 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #23 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #24 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #25 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 33 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #9 0x5580506875c8 in updateDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:53
    #10 0x558050687a58 in startDialogText /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:88
    #11 0x558050687d21 in startNewDialogInteraction /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:107
    #12 0x5580506885eb in Etf::DialogSystem::TextBoxInteractionUpdate(Etf::Textbox*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/systems/dialogSystem.cpp:187
    #13 0x558050655d7c in Etf::Textbox::Interact() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Textbox.cpp:38
    #14 0x55805060af23 in Etf::LocalPlayer::handleInteractions() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:145
    #15 0x55805060a113 in Etf::LocalPlayer::Update() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp:90
    #16 0x55805069f544 in Etf::GameObjectSystem::Update() /home/kevin/git/EscapeTheFate/src/systems/GameObjectSystem.cpp:55
    #17 0x558050601d1d in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:94
    #18 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #19 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #20 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #21 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #22 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #23 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #24 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #25 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #9 0x5580506629a6 in Etf::BattlerUI::UpdateHP(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/battle/battlerUI.cpp:95
    #10 0x55805064e7d3 in Etf::PlayerBattler::PlayerBattler(Etf::BattlerArgs const&) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:20
    #11 0x55805069148e in loadPlayers /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:130
    #12 0x558050692375 in loadBattle /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:187
    #13 0x558050692400 in triggerStateChange /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:204
    #14 0x5580506924e6 in Etf::BattleSystem::BattleSystemUpdate() /home/kevin/git/EscapeTheFate/src/systems/battleSystem.cpp:235
    #15 0x558050601d6e in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:98
    #16 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #17 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #18 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #19 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #20 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #21 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #22 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #23 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050686071 in Etf::UIText::UpdateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:36
    #9 0x5580506552bd in Etf::EnemyBattler::takeDamageImpl(int) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/EnemyBattler.cpp:24
    #10 0x558050611d39 in Etf::Battler::TakeDamage(int) /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Battler.cpp:37
    #11 0x558050650278 in Etf::PlayerBattler::handleInputTargetSelection() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:167
    #12 0x5580506505d6 in Etf::PlayerBattler::handleInput() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:190
    #13 0x55805064f4ed in Etf::PlayerBattler::updateImpl() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp:113
    #14 0x5580506125c5 in Etf::Battler::Update() /home/kevin/git/EscapeTheFate/src/gameobject/gameobjects/Battler.hpp:28
    #15 0x55805069f544 in Etf::GameObjectSystem::Update() /home/kevin/git/EscapeTheFate/src/systems/GameObjectSystem.cpp:55
    #16 0x558050601d1d in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:94
    #17 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83
    #18 0x5580509469e9 in SDL_AppIterate /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:133
    #19 0x7f8f3a035292  (/usr/lib/libSDL3.so.0+0x235292) (BuildId: 480c34a869a5798de3c6181fba76f0c5780f4030)
    #20 0x558050946450 in SDL_main /usr/include/SDL3/SDL_main_impl.h:59
    #21 0x55805094647b in main /usr/include/SDL3/SDL_main_impl.h:137
    #22 0x7f8f398366c0  (/usr/lib/libc.so.6+0x276c0) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #23 0x7f8f398367f8 in __libc_start_main (/usr/lib/libc.so.6+0x277f8) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #24 0x5580506012b4 in _start (/home/kevin/git/EscapeTheFate/build/bin/EscapeTheFate+0x992b4) (BuildId: 110ff9293af8cef127bfa1ce423c6a2686e6c840)

Indirect leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x7f8f3a720cb5 in malloc (/usr/lib/libasan.so.8+0x120cb5) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #1 0x7f8f39899bdf  (/usr/lib/libc.so.6+0x8abdf) (BuildId: 7a8d41a2df4fde040b4c6ac2832311ab645a1e41)
    #2 0x7f8f3a690a59 in vasprintf (/usr/lib/libasan.so.8+0x90a59) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #3 0x7f8f3a6920fd in asprintf (/usr/lib/libasan.so.8+0x920fd) (BuildId: 0b96d08695bbce2da9d4770c29ad2e72fb536f47)
    #4 0x55805096372b in TextureCreateRenderTargetImpl /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Platform/opengl/openglTexture.c:125
    #5 0x5580509528f3 in TextureCreateRenderTarget /home/kevin/git/EscapeTheFate/SupergoonEngine/src/Graphics/texture.c:35
    #6 0x558050957378 in TextRedrawText /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:300
    #7 0x5580509579c5 in TextOnDirty /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:358
    #8 0x558050957970 in TextLoad /home/kevin/git/EscapeTheFate/SupergoonEngine/src/text.c:353
    #9 0x5580506ab8e7 in Etf::Engine::TextBoi::CreateText(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, RectangleF, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, bool, bool) /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:348
    #10 0x558050685968 in Etf::UIText::UIText(Etf::UITextArgs&) /home/kevin/git/EscapeTheFate/src/ui/uiText.cpp:18
    #11 0x558050664512 in createText /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:73
    #12 0x558050666f69 in handleTypeCreation /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:169
    #13 0x558050667489 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:195
    #14 0x5580506671d2 in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:204
    #15 0x5580506685f4 in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #16 0x558050668624 in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #17 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #18 0x55805066872f in jforeach_lambda<handleUIArgs(const std::string&, json_object*)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #19 0x5580506675c8 in handleUIArgs /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:202
    #20 0x558050667c0f in operator() /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:248
    #21 0x5580506687fa in operator() /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:95
    #22 0x55805066882a in _FUN /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:93
    #23 0x558050945f4e in jforeach_obj /home/kevin/git/EscapeTheFate/SupergoonEngine/src/json.c:103
    #24 0x558050668935 in jforeach_lambda<Etf::UI::LoadUIFromFile(const std::string&)::<lambda(char const*, void*)> > /home/kevin/git/EscapeTheFate/src/bindings/engine.hpp:91
    #25 0x558050667fca in Etf::UI::LoadUIFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/kevin/git/EscapeTheFate/src/ui/ui.cpp:241
    #26 0x5580506a9abe in loadUI /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:121
    #27 0x5580506aa3c7 in Etf::Engine::HandleMapLoad() /home/kevin/git/EscapeTheFate/src/bindings/engine.cpp:201
    #28 0x558050601d11 in Etf::update() /home/kevin/git/EscapeTheFate/src/main.cpp:91
    #29 0x55805094669b in update /home/kevin/git/EscapeTheFate/SupergoonEngine/src/engine.c:83


## Backlogged

- More debug utils, copy from old repo
- BGM fadeout and fadein
- add enemy health bar and player health bar
- check on discord bot updates.
- More fields that can be optional to make it easier to write the jsonc file
  - not really needed right now
- Dialog system slidefade anim .. would be cool if we could handle tweening the opacity
- Better error handling / loading from json
- Use gameconfig to handle the dialog box variables, instead of const

## Version / Release Roadmap

- 0.2.x: local multiplayer
    : load all json at game load instead of on demand. - done
    : controllers and controller swapping - done
    : second player can spawn
    : camera follows main p1
    : second player can switch screens and interact with things
    : second battler
    : steam playtest - steamworks baby
    : use packed assets (.sg)

- 0.3.0: Main menu and splash screens
    : open a menu when exploring, should be small enough for both
    : settings screen in menu
    : simple viewing of stats
    : add in splash screens when loading the game
    : add in a welcome menu, where you can new game, options, etc
    : saving and loading

- 0.4.0: Battle enhancements
    : inventory that you can have (shared)
    : equipment that can be used
    : equip and change stats with the equip

## Previous releases

- 0.1.x: basic dialog with interactions, enter battle, can kill enemy, battle ended.
    : dialog - done
    : battle enter - done
    : battle finish - done
    : battle end - done
    : go through all todos and update - done
    : fix all leaks - done
