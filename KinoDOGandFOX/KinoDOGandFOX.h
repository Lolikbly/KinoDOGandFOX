#pragma execution_character_set("utf-8")
#pragma once

#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Collections::Generic;

public ref class Cinema {
public:
    int id;
    String^ name;
    String^ category;
    int capacity;
    int halls;
    String^ state;
};

public ref class Film {
public:
    int id;
    String^ title;
    String^ director;
    String^ genre;
    String^ studio;
};

public ref class Session {
public:
    int id;
    int cinema_id;
    int film_id;
    String^ date;
    String^ time;
    double price;
    int total_seats;
    int sold_tickets;
    int freeSeats() { return total_seats - sold_tickets; }
};

public ref class KinoDOGandFOX : public Form {
private:
    List<Cinema^>^ cinemas;
    List<Film^>^ films;
    List<Session^>^ sessions;
    int nextCinemaId;
    int nextFilmId;
    int nextSessionId;
    String^ dataPath;

    // Вспомогательные методы
    void EnsurePath();
    void LoadCinemas();
    void SaveCinemas();
    void LoadFilms();
    void SaveFilms();
    void LoadSessions();
    void SaveSessions();
    void CreateTestData();

    // Методы для работы с гридами
    void LoadCinemasGrid();
    void LoadFilmsGrid();
    void LoadSessionsGrid();

    // Обработчики событий
    void OnFormResize(Object^ sender, EventArgs^ e);
    void OnAddCinema(Object^ sender, EventArgs^ e);
    void OnEditCinema(Object^ sender, EventArgs^ e);
    void OnDeleteCinema(Object^ sender, EventArgs^ e);
    void OnAddFilm(Object^ sender, EventArgs^ e);
    void OnEditFilm(Object^ sender, EventArgs^ e);
    void OnDeleteFilm(Object^ sender, EventArgs^ e);
    void OnAddSession(Object^ sender, EventArgs^ e);
    void OnEditSession(Object^ sender, EventArgs^ e);
    void OnDeleteSession(Object^ sender, EventArgs^ e);
    void OnShowRepertoire(Object^ sender, EventArgs^ e);
    void OnSaveRepertoire(Object^ sender, EventArgs^ e);
    void OnShowFreeSeats(Object^ sender, EventArgs^ e);
    void OnSaveFreeResult(Object^ sender, EventArgs^ e);
    void OnShowSoldTickets(Object^ sender, EventArgs^ e);
    void OnSaveSoldReport(Object^ sender, EventArgs^ e);
    void OnExit(Object^ sender, EventArgs^ e);

    // Элементы управления
    TabControl^ tabControl;
    TabPage^ tabAdmin;
    TabPage^ tabRepertoire;
    TabPage^ tabFreeSeats;
    TabPage^ tabSoldTickets;

    ComboBox^ cmbCinemaRep;
    DateTimePicker^ dtpRepDate;
    Button^ btnShowRep;
    RichTextBox^ txtRep;
    Button^ btnSaveRep;

    ComboBox^ cmbCinemaFree;
    DateTimePicker^ dtpFreeDate;
    DateTimePicker^ dtpFreeTime;
    Button^ btnShowFree;
    Label^ lblFree;
    Button^ btnSaveFree;

    ComboBox^ cmbFilmSold;
    DateTimePicker^ dtpSoldStart;
    DateTimePicker^ dtpSoldEnd;
    Button^ btnShowSold;
    Label^ lblSold;
    Button^ btnSaveSold;

    DataGridView^ dgvCinemas;
    DataGridView^ dgvFilms;
    DataGridView^ dgvSessions;
    Button^ btnAddCinema;
    Button^ btnEditCinema;
    Button^ btnDeleteCinema;
    Button^ btnAddFilm;
    Button^ btnEditFilm;
    Button^ btnDeleteFilm;
    Button^ btnAddSession;
    Button^ btnEditSession;
    Button^ btnDeleteSession;
    Button^ btnExit;

public:
    KinoDOGandFOX();

    void SaveAll();
    void AddCinema(String^ name, String^ cat, int cap, int halls, String^ state);
    List<Cinema^>^ GetCinemas();
    array<String^>^ GetCinemaNames();
    int GetCinemaId(String^ name);
    String^ GetCinemaNameById(int id);

    void AddFilm(String^ title, String^ dir, String^ genre, String^ studio);
    List<Film^>^ GetFilms();
    array<String^>^ GetFilmTitles();
    int GetFilmId(String^ title);
    String^ GetFilmTitleById(int id);

    void AddSession(int cid, int fid, String^ date, String^ time, double price, int total, int sold);

    void UpdateCinema(int index, String^ name, String^ cat, int cap, int halls, String^ state);
    void UpdateFilm(int index, String^ title, String^ dir, String^ genre, String^ studio);
    void UpdateSession(int index, int cinema_id, int film_id, String^ date, String^ time, double price, int total, int sold);

    void DeleteCinemaAtIndex(int index);
    void DeleteFilmAtIndex(int index);
    void DeleteSessionAtIndex(int index);

    List<Session^>^ GetSessions();

    List<String^>^ GetRepertoireByDate(int cinemaId, String^ date);
    int GetFreeSeats(int cinemaId, String^ date, String^ time);
    int GetSoldTicketsForFilmPeriod(String^ filmTitle, String^ startDate, String^ endDate);
    void SaveReportToFile(String^ filename, List<String^>^ lines);

    void RefreshAll();
    void InitializeComponent();
};