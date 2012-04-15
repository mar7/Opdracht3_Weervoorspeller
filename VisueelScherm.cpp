/*
 * VisueelScherm.cpp
 *
 *  Created on: 11 mrt. 2012
 *      Author: acer, Marijn
 */


#include "VisueelScherm.hpp"

VisueelScherm::VisueelScherm( WeerData* weerData )
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

	//maak een listbox met update en textueelknop
	this->listBox = new ListBox(0, 260, screenWidth, screenHeight, achtergrond, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true);
	//listBox->setPaddingTop(3);
	listBox->setPaddingLeft(listBox->getWidth() / 2 - 60);
	listBox->setBackgroundColor(0xffffff);

	//knop om data te updaten
	this->updateKnop = new Label( 0, 0, 120, 30, NULL, "    Update", 0, font );
	updateKnop->setPaddingTop(5);
	updateKnop->setPaddingLeft(10);
	this->updateKnop->setSkin( this->skin );
	listBox->add(updateKnop);

	//knop om naar textueel te schakelen
	this->textueelKnop = new Label( 0, 0, 120, 30, NULL, "Textuele weergave", 0, font );
	textueelKnop->setPaddingTop(5);
	textueelKnop->setPaddingLeft(7);
	this->textueelKnop->setSkin( this->skin );
	listBox->add(textueelKnop);


	//staafdiagram
	//maak eerst een placeholder
	this->diagramTekening = maCreatePlaceholder();

	//laat de placeholder tekenbaar zijn
	maCreateDrawableImage( this->diagramTekening, EXTENT_X( maGetScrSize() ), 255 );

	//mak een nieuwe image met de placeholder
	this->diagramImage = new Image( 0, 0, EXTENT_X( maGetScrSize() ), 260, achtergrond, true, true, this->diagramTekening );
	//diagramImage->setBackgroundColor(0xffffff);

	this->setMain( achtergrond );

	this->update(); // direct update doen om diagram te tonen
}


VisueelScherm::~VisueelScherm()
{
}


void VisueelScherm::update()
{
	//update waarden vam weerdata
	this->weerData->update();

	//stel draw target in op onze tekening
	maSetDrawTarget( this->diagramTekening );

	//legenda
	//teken blokje en text met zonneschijn, neerslag en temperatuur

	// zonneschijn in oranje
	maSetColor(0xff8c00);
	maFillRect(10, 205, 10, 10);
	maDrawText(25, 200, "Zonneschijn (%)");

	// neerslag in blauw
	maSetColor(0x00ced1);
	maFillRect(10, 220, 10, 10);
	maDrawText(25, 215, "Neerslag (%) ");

	// temperatuur in groen
	maSetColor(0x32cd32);
	maFillRect(10, 235, 10, 10);
	maDrawText(25, 230, "Min. Temp. (graden C)");

	// bijschrift assen in wit
	maSetColor(0xffffff);
	maDrawText(2, 1, "100");
	maDrawText(5, 65, "50");
	maDrawText(5, 111, "20");
	maDrawText(8, 140, "0");
	maDrawText(2, 155, "-10");
	maDrawText(2, 170, "-20");
	maDrawText(50, 180, "Nu");
	maDrawText(110, 180, "Morg.");
	maDrawText(180, 180, "Overm.");

	//teken 10-stap lijnen
	maSetColor(0x4169e1); // kleur blauw maken
	for (int i = 1; i < 13; i++) // alle lijnen van het diagram tekenen
	{
		maLine(27, i * 15, 230, i * 15);
	}

	// teken witte assen staafdiagram
	maSetColor(0xffffff);
	maLine(27, 5, 27, 185); // y-as
	maLine(27, 150, 230, 150); // x-as


	//teken de staven van zonneschijn, neerslag en minimum temperatuur

	// staven zonneschijn
	maSetColor(0xff8c00);
	maFillRect(50, 150 - (weerData->zonneschijn[0] / 10 * 15), 5, (weerData->zonneschijn[0] / 10 * 15));
	maFillRect(110, 150 - (weerData->zonneschijn[1] / 10 * 15), 5, (weerData->zonneschijn[1] / 10 * 15));
	maFillRect(180, 150 - (weerData->zonneschijn[2] / 10 * 15), 5, (weerData->zonneschijn[2] / 10 * 15));

	// staven neerslag
	maSetColor(0x00ced1);
	maFillRect(60, 150 - (weerData->neerslag[0] / 10 * 15), 5, (weerData->neerslag[0] / 10 * 15));
	maFillRect(120, 150 - (weerData->neerslag[1] / 10 * 15), 5, (weerData->neerslag[1] / 10 * 15));
	maFillRect(190, 150 - (weerData->neerslag[2] / 10 * 15), 5, (weerData->neerslag[2] / 10 * 15));

	// staven minimum temperatuur
	maSetColor(0x32cd32);

	// tekenen afhankelijk van positieve/negatieve waarde
	if(weerData->minimumtemperatuur[0] < 0)
	{
		maFillRect(70, 150, 5, (weerData->minimumtemperatuur[0] / 10 * -15));
	}
	else
	{
		maFillRect(70, 150 - (weerData->minimumtemperatuur[0] / 10 * 15), 5, (weerData->minimumtemperatuur[0] / 10 * 15));
	}

	if(weerData->minimumtemperatuur[1] < 0) // bij negatieve temp anders tekenen
	{
		maFillRect(130, 150, 5, (weerData->minimumtemperatuur[1] / 10 * -15));
	}
	else
	{
		maFillRect(130, 150 - (weerData->minimumtemperatuur[1] / 10 * 15), 5, (weerData->minimumtemperatuur[1] / 10 * 15));
	}

	if(weerData->minimumtemperatuur[2] < 0) // bij negatieve temp anders tekenen
	{
		maFillRect(200, 150, 5, (weerData->minimumtemperatuur[2] / 10 * -15));
	}
	else
	{
		maFillRect(200, 150 - (weerData->minimumtemperatuur[2] / 10 * 15), 5, (weerData->minimumtemperatuur[2] / 10 * 15));
	}

	//altijd draw target na tekenen teruggeven naar scherm!
	maSetDrawTarget( HANDLE_SCREEN );

	//update de image met de nieuwe tekening
	this->diagramImage->setResource( this->diagramTekening );
}


void VisueelScherm::setToggleScherm( Screen* toggleScherm )
{
	//switch naar andere scherm (textueel scherm)
	this->toggleScherm = toggleScherm;
}


void VisueelScherm::pointerPressEvent( MAPoint2d maPoint )
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


	//textueel knop is ingedrukt
	if( this->textueelKnop->contains( point ) )
	{
		this->textueelKnop->setSelected( true );
		this->hide();
		this->toggleScherm->show();
	}
	else
	{
		this->textueelKnop->setSelected( false );
	}
}
