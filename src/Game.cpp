#include "Game.h"

Game::Game()
{
    m_settings;
    m_settings.antialiasingLevel = 8;
    InitWindow();
}

Game::~Game()
{
    ImGui::SFML::Shutdown();
}

void Game::Run()
{
    sf::Clock deltaClock;

    sf::Color bgColor;
    static float bgColorArr[3] = { 0.f, 0.f, 0.f };
    sf::Color beamColor;
    static float beamColorArr[3] = { 0.f, 0.f, 0.f };

    char windowTitle[255] = "raycast";
    m_window->setTitle( windowTitle );

    while ( m_window->isOpen() )
    {
        sf::Event Event;
        while ( m_window->pollEvent( Event ) )
        {
            ImGui::SFML::ProcessEvent( Event );
            if ( Event.type == sf::Event::Closed )
            {
                m_window->close();
            }
            if ( Event.type == sf::Event::MouseMoved )
            {
                Point newPosition = sf::Mouse::getPosition( *m_window.get() );
                m_source.Move( newPosition );
            }
        }

        ImGui::SFML::Update( *m_window.get(), deltaClock.restart() );

        ImGui::Begin( "Background color edit" ); // create the UI window

        // Background color picking tool
        if ( ImGui::ColorEdit3( "Background color", bgColorArr ) )
        {
            // This code is called whenever the color data has been changed
            bgColor.r = static_cast<sf::Uint8>(bgColorArr[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(bgColorArr[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(bgColorArr[2] * 255.f);
        }
        // Ray color picking tool
        if ( ImGui::ColorEdit3( "Beam color", beamColorArr ) )
        {
            // This code is called whenever the color data has been changed
            beamColor.r = static_cast<sf::Uint8>(beamColorArr[0] * 255.f);
            beamColor.g = static_cast<sf::Uint8>(beamColorArr[1] * 255.f);
            beamColor.b = static_cast<sf::Uint8>(beamColorArr[2] * 255.f);
            beamColor.a = 50;
            m_source.SetBeamColor( beamColor );
        }

        // select number of beams
        static int beamCountSliderValue = Source::DEFAULT_BEAM_COUNT;
        if ( ImGui::SliderInt( "Beam count", &beamCountSliderValue, 0, 1080 ) )
        {
            m_source.SetBeamCount( beamCountSliderValue );
        }

        // select number of reflections
        static int reflectionsSliderValue = 0;
        if ( ImGui::SliderInt( "Reflections depth", &reflectionsSliderValue, 0, 10 ) )
        {
            m_source.SetReflectionsNumber( reflectionsSliderValue );
        }

        ImGui::End(); // end window

        m_window->clear( bgColor ); // Fill the window with the selected color
        ImGui::SFML::Render( *m_window.get() );

        m_source.CheckWalls( m_walls );
        Redraw();
    }
}

void Game::Redraw()
{
    //m_window->clear( sf::Color::Black );
    m_source.Draw( *m_window.get() );

    for ( int i = 0; i < m_walls.size(); ++i )
    {
        m_walls[i].Draw( *m_window.get() );
    }

    m_window->display();
}

void Game::InitWindow()
{
    m_window = std::make_unique<sf::RenderWindow>( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ),
                                                   "raycast", sf::Style::Close, m_settings );

    ImGui::SFML::Init( *m_window.get() );
}

void Game::AddWall( Point& start, Point& end )
{
    m_walls.emplace_back( start, end );
}

void Game::AddWall( Point&& start, Point&& end )
{
    m_walls.emplace_back( start, end );
}
