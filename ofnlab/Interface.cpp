#include "Interface.hpp"
#include <ofn++\Variable.hpp>
#include <ofn++\Parser.hpp>
#include <iostream>

namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

    bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size(), 9);
    }
}

void setStyle()
{
    ImGuiStyle * style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(15, 5);
    style->WindowRounding = 1.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 3.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 6.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;
    style->ColumnsMinSpacing = 0.f;

    auto windowBg = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
    auto inputBg  = ImVec4(0.25f, 0.25f, 0.25f, 1.f);
    auto headerBg = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
    auto buttonBg = ImVec4(0.25f, 0.25f, 0.25f, 1.f);

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = windowBg;
    style->Colors[ImGuiCol_ChildWindowBg] = windowBg;
    style->Colors[ImGuiCol_PopupBg] = windowBg;
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = inputBg;
    style->Colors[ImGuiCol_FrameBgHovered] = inputBg;
    style->Colors[ImGuiCol_FrameBgActive] = inputBg;

    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);

    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

    style->Colors[ImGuiCol_Button] = buttonBg;
    style->Colors[ImGuiCol_ButtonHovered] = headerBg;
    style->Colors[ImGuiCol_ButtonActive] = buttonBg;

    style->Colors[ImGuiCol_Header] = headerBg;
    style->Colors[ImGuiCol_HeaderHovered] = headerBg;
    style->Colors[ImGuiCol_HeaderActive] = headerBg;

    style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);

    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(.80f, .80f, 0.80f, 0.2f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.80f);
}

Interface::Interface(sf::RenderWindow& window, Grid& grid)
{
    m_window = &window;
    ImGui::SFML::Init(window);
    m_grid = &grid;
    m_range[0] = -10;
    m_range[1] = 10;
    updateGrid();

    m_texture.loadFromFile("assets/logo.png");
    m_sprite.setTexture(m_texture);

    setStyle();
}

Interface::~Interface()
{
    ImGui::Shutdown();
}

void Interface::update(sf::Event& event)
{
    ImGui::SFML::ProcessEvent(event);
}

void Interface::update()
{
    m_delta = m_clock.restart();
    ImGui::SFML::Update(*m_window, m_delta);
  
    ImGui::SetNextWindowPos(ImVec2(m_window->getSize().x - 330, 0), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(330, m_window->getSize().y), ImGuiSetCond_Always);
    ImGui::Begin("OFNLAB version 1.0", nullptr, 
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar
    );
    ImGui::Spacing();
    ImGui::Image(m_sprite);
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("About")) {
        ImGui::TextColored(ImVec4(0.f, 0.8f, 1.f, 1.f), "Ofnlab ver. 0.1.0");
        ImGui::Separator();
        ImGui::Text("Author: Damian Barczynski");
        ImGui::Text("E-Mail: kontakt.daan@gmail.com");
        ImGui::Text("Github: www.github.com/daancode");
        if (ImGui::Button("Exit", ImVec2(300, 30))) {
            m_window->close();
        }
    }

    if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::DragInt2("Plot range", m_range)) {
            updateGrid();
        }
    }

    if (ImGui::CollapsingHeader("Parser", ImGuiTreeNodeFlags_DefaultOpen)) {
        calculationDialog();
    }

    std::vector<std::string> varLabels = { "(None)" };
    for (auto& wrapper : m_wrappers) {
        varLabels.push_back(wrapper.m_varname);
    }

    if (ImGui::CollapsingHeader("Variable editor", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Columns(2, nullptr, false);
        ImGui::PushItemWidth(-1.f);
        ImGui::ListBox("", &currentIndex, varLabels);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1.f);

        int btnWidth = -1;
        if (ImGui::Button("Add", ImVec2(btnWidth, 30))) {
            ImGui::OpenPopup("Add new variable");
        }
        addNewVariable();
        if (ImGui::Button("Remove", ImVec2(btnWidth, 30))) {
            if (!m_wrappers.empty() && currentIndex != 0) {
                m_wrappers.erase(m_wrappers.begin() + (currentIndex - 1));
                currentIndex = m_wrappers.size();
            }
        }
        if (ImGui::Button("Copy", ImVec2(btnWidth, 30))) {
            if (!m_wrappers.empty() && currentIndex != 0) {
                m_wrappers.push_back(m_wrappers[currentIndex - 1]);
                strcpy(m_wrappers.back().m_varname, "Copy");
            }
        }
        if (ImGui::Button("Hide all", ImVec2(btnWidth, 30))) {
            for (auto& wrapper : m_wrappers) {
                wrapper.m_visible = false;
                wrapper.update();
            }
        }
        if (ImGui::Button("Show all", ImVec2(btnWidth, 30))) {
            for (auto& wrapper : m_wrappers) {
                wrapper.m_visible = true;
                wrapper.update();
            }
        }
        ImGui::PopItemWidth();
        ImGui::Columns(1);
        ImGui::Spacing();

        if (!m_wrappers.empty() && currentIndex > 0) {
            auto& wrapper = m_wrappers[currentIndex - 1];
            float color[] = {
                wrapper.m_color.r / 255.f,
                wrapper.m_color.g / 255.f,
                wrapper.m_color.b / 255.f
            };
            float value[] = {
                wrapper.m_var[0],
                wrapper.m_var[ofn::Variable::discretization.ratio],
                wrapper.m_var[ofn::Variable::discretization.ratio + 1],
                wrapper.m_var[ofn::Variable::discretization.size - 1]
            };
            
            if (ImGui::CollapsingHeader("Edit variable", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputText("Var name", wrapper.m_varname, 5);
                ImGui::InputFloat4("Value", value, 2, ImGuiInputTextFlags_ReadOnly);
                if (ImGui::ColorEdit3("Set color", color)) {
                    wrapper.m_color = {
                        static_cast<sf::Uint8>(color[0] * 255.f),
                        static_cast<sf::Uint8>(color[1] * 255.f),
                        static_cast<sf::Uint8>(color[2] * 255.f)
                    };
                    wrapper.update();
                }
                if (ImGui::Button("Re-order")) {
                    wrapper.m_var.changeOrder();
                    wrapper.update();
                }
                ImGui::SameLine();
                if (ImGui::Button("Invert")) {
                    wrapper.m_var = 1.f / wrapper.m_var;
                    wrapper.update();
                }
                ImGui::SameLine();
                ImGui::Checkbox("Visible", &wrapper.m_visible);
            }
        }
    }

    ImGui::End();
}

void Interface::display()
{
    for (auto& wrapper : m_wrappers) {
        if (wrapper.m_visible) {
            m_window->draw(wrapper);
        }
    }
    ImGui::Render();
}

void Interface::addNewVariable()
{
    if (ImGui::BeginPopupModal("Add new variable", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static WrapperInitializer initializer;
        ImGui::InputText("Set name", initializer.name, 5);
        ImGui::ColorEdit3("Set color", initializer.color);
        ImGui::InputFloat4("Set values", initializer.value, 2);

        ImGui::Columns(2, nullptr, false);
        ImGui::Checkbox("Visible", &initializer.visible);
        ImGui::NextColumn();
        if (ImGui::Button("Add", ImVec2(60, 0))) {
            ImGui::CloseCurrentPopup();
            Wrapper wrapper{ *m_grid,
                ofn::Variable(
                    initializer.value[0],
                    initializer.value[1],
                    initializer.value[2],
                    initializer.value[3]
                ),
                {
                    static_cast<sf::Uint8>(initializer.color[0] * 255.f),
                    static_cast<sf::Uint8>(initializer.color[1] * 255.f),
                    static_cast<sf::Uint8>(initializer.color[2] * 255.f)
                }
            };
            strcpy(wrapper.m_varname, initializer.name);
            m_wrappers.push_back(wrapper);
            initializer = WrapperInitializer();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(60, 0))) {
            ImGui::CloseCurrentPopup();
            initializer = WrapperInitializer();
        }
        ImGui::Columns(1);
        ImGui::EndPopup();
    }
}

void Interface::calculationDialog()
{
    static int id = 0;
    static char buf[40];
    static std::string error;
    static bool hideVars = false;

    ImGui::Text("Enter expression to prase");
    ImGui::PushItemWidth(-1.f);
    if (ImGui::InputText("", buf, 40, ImGuiInputTextFlags_EnterReturnsTrue)) {
        error.clear();
        ofn::Parser parser;
        for (auto& number : m_wrappers) {
            parser.addVariable(number.m_varname, number.m_var);
        }
        try {
            auto result = parser.parse(buf);
            Wrapper wrapper(*m_grid, result, sf::Color::Red);
            wrapper.m_varname[0] = 'X';
            strcpy(wrapper.m_varname + 1, std::to_string(id++).data());
            if (hideVars) {
                for (auto& w : m_wrappers) {
                    w.m_visible = false;
                }
            }
            wrapper.m_visible = true;
            m_wrappers.push_back(wrapper);
            currentIndex = m_wrappers.size();
        }
        catch (std::string& err) {
            error = err;
        }
    }
    ImGui::PopItemWidth();

    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(450.0f);
        ImGui::TextUnformatted("Press 'Enter' to calculate");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    if (!error.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.f, 0.f, 1.f), "Parsing error !");
        ImGui::TextColored(ImVec4(1.0f, 0.f, 0.f, 1.f), error.data());
    }

    ImGui::Checkbox("Hide other variables", &hideVars);
}

void Interface::updateGrid()
{
    m_grid->create(
        { (float)m_window->getSize().x - 380, (float)m_window->getSize().y - 50 },
        { m_range[0], m_range[1] }
    );
    for (auto& wrapper : m_wrappers) {
        wrapper.update();
    }
}