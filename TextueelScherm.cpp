/*
 * TextueelScherm.cpp
 *
 *  Created on: 11 mrt. 2012
 *      Author: acer
 */

#include "TextueelScherm.hpp"

TextueelScherm::TextueelScherm( WeerData* weerData )
{
	//sla de weerdata op in het attribuut
	this->weerData = weerData;

	// schermgrootte
	MAExtent screenSize = maGetScrSize();
	int screenWidth = EXTENT_X(screenSize);
	int screenHeight = EXTENT_Y(screenSize);

	//ken font en skin toe
	this->font = new MAUI::Font(RES_FONT);
	this->skin = new WidgetSkin( RES_SELECTED, RES_UNSELECTED, 16, 32, 16, 32, false, false );

	//maak een achtergrond label om alle andere widgets in op te slaan, en te tonen
	Label* achtergrond = new Label(0, 0, 0, 0, NULL);
	achtergrond->setBackgroundColor(0xffffff);

	//maak een listbox waar update en visueelknop aan toegevoegd worden
	this->listBox = new ListBox(0, 190, screenWidth, screenHeight, achtergrond);

	//knop om data te updaten
	this->updateKnop = new Label( 30, 200, 80, 30, NULL, "Update", 0, font );
	updateKnop->setPaddingTop(5);
	updateKnop->setPaddingLeft(10);
	this->updateKnop->setSkin( this->skin );
	listBox->add(updateKnop);

	//knop om naar visueel scherm te schakelen
	this->visueelKnop = new Label( 30, 240, 120, 30, NULL, "Visuele weergave", 0, font );
	visueelKnop->setPaddingTop(5);
	visueelKnop->setPaddingLeft(10);
	this->visueelKnop->setSkin( this->skin );
	listBox->add(visueelKnop);


	//label veld voor weer data, beslaat de rest van het scherm (zie onder)
	this->weerDataLabel = new Label( 10, 10, screenWidth - 5, 30, achtergrond, "3-daagse weersverwachting", 0, font );
	//weerDataLabel->setBackgroundColor(0x0f0f0f);
	weerDataLabel->setSkin( this->skin );

	//info label
	this->weerInfo = new Label( 10, 30, 200, 30, achtergrond, "Vandaag  Morgen Overmorgen", 0, font );

	//zonneschijn label
	this->weerInfoZon = new Label( 10, 70, 200, 30, achtergrond, "", 0, font );

	//neerslag label
	this->weerInfoNeerslag = new Label( 10, 110, 200, 30, achtergrond, "", 0, font );

	//minimum temperatuur label
	this->weerInfoMintemp = new Label( 10, 150, 200, 30, achtergrond, "", 0, font );

	//update labels met text, etc.
	this->update();

	//stel achtergrond label in als main widget, en maakt alles zichtbaar
	this->setMain(achtergrond);
}

TextueelScherm::~TextueelScherm()
{
}



void TextueelScherm::toonWeerData()
{
	//tijdelijke data buffer voor sprintf operaties
	char data[500];

	//stel tekst in voor info label
	sprintf( data, "Data: Nu / Morgen / Overmorgen" );
	this->weerInfo->setCaption(data);

	//stel tekst in voor zonneschijn label
	sprintf( data, "Zonneschijn: %i%%	/ %i%% / %i%%", weerData->zonneschijn[0], weerData->zonneschijn[1], weerData->zonneschijn[2] );
	this->weerInfoZon->setCaption( data );

	//stel tekst in voor neerslag label
	sprintf( data, "Neerslag: %i%% / %i%% / %i%%", weerData->neerslag[0], weerData->neerslag[1], weerData->neerslag[2] );
	this->weerInfoNeerslag->setCaption( data );

	//stel tekst in voor minimum temperatuur label
	sprintf( data, "Min. Temp.: %i / %i / %i", weerData->minimumtemperatuur[0], weerData->minimumtemperatuur[1], weerData->minimumtemperatuur[2]  );
	this->weerInfoMintemp->setCaption( data );
}


void TextueelScherm::setToggleScherm( Screen* toggleScherm )
{
	//switch naar andere scherm (visueel scherm)
	this->toggleScherm = toggleScherm;
}


void TextueelScherm::update()
{
	//update weerdata en geef alles weer (update labels met nieuwe data)
	this->weerData->update();
	this->toonWeerData();
}


void TextueelScherm::pointerPressEvent( MAPoint2d maPoint )
{
	Point point;
	point.x = maPoint.x;
	point.y = maPoint.y;

	//update knop is ingedrukt
	if( this->updateKnop->contains( point ) )
	{
		this->updateKnop->setSelected( true );
		this->update();
	}
	else
	{
		this->updateKnop->setSelected( false );
	}


	//visueel knop is ingedrukt
	if( this->visueelKnop->contains( point ) )
	{
		this->visueelKnop->setSelected( true );
		this->hide();
		this->toggleScherm->show();
	}
	else
	{
		this->visueelKnop->setSelected( false );
	}
}


