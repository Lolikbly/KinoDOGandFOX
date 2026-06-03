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

    void EnsurePath() {
        dataPath = Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData) + "\\KinoDOGandFOX\\Data";
        if (!Directory::Exists(dataPath)) Directory::CreateDirectory(dataPath);
    }

    void LoadCinemas() {
        String^ file = dataPath + "\\cinemas.txt";
        if (!File::Exists(file)) return;
        cinemas->Clear();
        array<String^>^ lines = File::ReadAllLines(file, System::Text::Encoding::UTF8);
        for each(String ^ line in lines) {
            array<String^>^ p = line->Split(',');
            if (p->Length >= 6) {
                Cinema^ c = gcnew Cinema();
                c->id = Int32::Parse(p[0]);
                c->name = p[1];
                c->category = p[2];
                c->capacity = Int32::Parse(p[3]);
                c->halls = Int32::Parse(p[4]);
                c->state = p[5];
                cinemas->Add(c);
                if (c->id >= nextCinemaId) nextCinemaId = c->id + 1;
            }
        }
    }

    void SaveCinemas() {
        List<String^>^ lines = gcnew List<String^>();
        for each(Cinema ^ c in cinemas) {
            lines->Add(String::Format("{0},{1},{2},{3},{4},{5}",
                c->id, c->name, c->category, c->capacity, c->halls, c->state));
        }
        File::WriteAllLines(dataPath + "\\cinemas.txt", lines, System::Text::Encoding::UTF8);
    }

    void LoadFilms() {
        String^ file = dataPath + "\\films.txt";
        if (!File::Exists(file)) return;
        films->Clear();
        array<String^>^ lines = File::ReadAllLines(file, System::Text::Encoding::UTF8);
        for each(String ^ line in lines) {
            array<String^>^ p = line->Split(',');
            if (p->Length >= 5) {
                Film^ f = gcnew Film();
                f->id = Int32::Parse(p[0]);
                f->title = p[1];
                f->director = p[2];
                f->genre = p[3];
                f->studio = p[4];
                films->Add(f);
                if (f->id >= nextFilmId) nextFilmId = f->id + 1;
            }
        }
    }

    void SaveFilms() {
        List<String^>^ lines = gcnew List<String^>();
        for each(Film ^ f in films) {
            lines->Add(String::Format("{0},{1},{2},{3},{4}",
                f->id, f->title, f->director, f->genre, f->studio));
        }
        File::WriteAllLines(dataPath + "\\films.txt", lines, System::Text::Encoding::UTF8);
    }

    void LoadSessions() {
        String^ file = dataPath + "\\sessions.txt";
        if (!File::Exists(file)) return;
        sessions->Clear();
        array<String^>^ lines = File::ReadAllLines(file, System::Text::Encoding::UTF8);
        for each(String ^ line in lines) {
            array<String^>^ p = line->Split(',');
            if (p->Length >= 8) {
                Session^ s = gcnew Session();
                s->id = Int32::Parse(p[0]);
                s->cinema_id = Int32::Parse(p[1]);
                s->film_id = Int32::Parse(p[2]);
                s->date = p[3];
                s->time = p[4];
                s->price = Double::Parse(p[5]);
                s->total_seats = Int32::Parse(p[6]);
                s->sold_tickets = Int32::Parse(p[7]);
                sessions->Add(s);
                if (s->id >= nextSessionId) nextSessionId = s->id + 1;
            }
        }
    }

    void SaveSessions() {
        List<String^>^ lines = gcnew List<String^>();
        for each(Session ^ s in sessions) {
            lines->Add(String::Format("{0},{1},{2},{3},{4},{5},{6},{7}",
                s->id, s->cinema_id, s->film_id, s->date, s->time, s->price, s->total_seats, s->sold_tickets));
        }
        File::WriteAllLines(dataPath + "\\sessions.txt", lines, System::Text::Encoding::UTF8);
    }

    void CreateTestData() {
        AddCinema("Космос", "Люкс", 300, 4, "работает");
        AddCinema("Родина", "Стандарт", 200, 2, "работает");
        AddFilm("Начало", "Нолан Кристофер", "фантастика", "Warner");
        AddFilm("Форрест Гамп", "Земекис Роберт", "драма", "Paramount");
        AddSession(1, 1, "2025-06-01", "18:30", 350, 300, 250);
        AddSession(1, 1, "2025-06-01", "21:00", 400, 300, 180);
        AddSession(2, 2, "2025-06-01", "19:00", 300, 200, 120);
        SaveAll();
    }

public:
    KinoDOGandFOX() {
        cinemas = gcnew List<Cinema^>();
        films = gcnew List<Film^>();
        sessions = gcnew List<Session^>();
        nextCinemaId = 1;
        nextFilmId = 1;
        nextSessionId = 1;
        dataPath = nullptr;

        EnsurePath();
        LoadCinemas();
        LoadFilms();
        LoadSessions();
        if (cinemas->Count == 0 && films->Count == 0 && sessions->Count == 0) {
            CreateTestData();
        }
        InitializeComponent();
    }

    void SaveAll() {
        SaveCinemas();
        SaveFilms();
        SaveSessions();
    }

    void AddCinema(String^ name, String^ cat, int cap, int halls, String^ state) {
        Cinema^ c = gcnew Cinema();
        c->id = nextCinemaId++;
        c->name = name;
        c->category = cat;
        c->capacity = cap;
        c->halls = halls;
        c->state = state;
        cinemas->Add(c);
        SaveCinemas();
    }

    List<Cinema^>^ GetCinemas() { return cinemas; }

    array<String^>^ GetCinemaNames() {
        array<String^>^ a = gcnew array<String^>(cinemas->Count);
        for (int i = 0; i < cinemas->Count; i++) a[i] = cinemas[i]->name;
        return a;
    }

    int GetCinemaId(String^ name) {
        for each(Cinema ^ c in cinemas) if (c->name == name) return c->id;
        return -1;
    }

    String^ GetCinemaNameById(int id) {
        for each(Cinema ^ c in cinemas) if (c->id == id) return c->name;
        return "?";
    }

    void AddFilm(String^ title, String^ dir, String^ genre, String^ studio) {
        Film^ f = gcnew Film();
        f->id = nextFilmId++;
        f->title = title;
        f->director = dir;
        f->genre = genre;
        f->studio = studio;
        films->Add(f);
        SaveFilms();
    }

    List<Film^>^ GetFilms() { return films; }

    array<String^>^ GetFilmTitles() {
        array<String^>^ a = gcnew array<String^>(films->Count);
        for (int i = 0; i < films->Count; i++) a[i] = films[i]->title;
        return a;
    }

    int GetFilmId(String^ title) {
        for each(Film ^ f in films) if (f->title == title) return f->id;
        return -1;
    }

    String^ GetFilmTitleById(int id) {
        for each(Film ^ f in films) if (f->id == id) return f->title;
        return "?";
    }

    void AddSession(int cid, int fid, String^ date, String^ time, double price, int total, int sold) {
        Session^ s = gcnew Session();
        s->id = nextSessionId++;
        s->cinema_id = cid;
        s->film_id = fid;
        s->date = date;
        s->time = time;
        s->price = price;
        s->total_seats = total;
        s->sold_tickets = sold;
        sessions->Add(s);
        SaveSessions();
    }

    void UpdateCinema(int index, String^ name, String^ cat, int cap, int halls, String^ state) {
        if (index >= 0 && index < cinemas->Count) {
            cinemas[index]->name = name;
            cinemas[index]->category = cat;
            cinemas[index]->capacity = cap;
            cinemas[index]->halls = halls;
            cinemas[index]->state = state;
            SaveCinemas();
        }
    }

    void UpdateFilm(int index, String^ title, String^ dir, String^ genre, String^ studio) {
        if (index >= 0 && index < films->Count) {
            films[index]->title = title;
            films[index]->director = dir;
            films[index]->genre = genre;
            films[index]->studio = studio;
            SaveFilms();
        }
    }

    void UpdateSession(int index, int cinema_id, int film_id, String^ date, String^ time, double price, int total, int sold) {
        if (index >= 0 && index < sessions->Count) {
            sessions[index]->cinema_id = cinema_id;
            sessions[index]->film_id = film_id;
            sessions[index]->date = date;
            sessions[index]->time = time;
            sessions[index]->price = price;
            sessions[index]->total_seats = total;
            sessions[index]->sold_tickets = sold;
            SaveSessions();
        }
    }

    void DeleteCinemaAtIndex(int index) {
        if (index >= 0 && index < cinemas->Count) {
            Cinema^ c = cinemas[index];
            for (int i = sessions->Count - 1; i >= 0; i--) {
                if (sessions[i]->cinema_id == c->id) {
                    sessions->RemoveAt(i);
                }
            }
            cinemas->RemoveAt(index);
            SaveAll();
        }
    }

    void DeleteFilmAtIndex(int index) {
        if (index >= 0 && index < films->Count) {
            Film^ f = films[index];
            for (int i = sessions->Count - 1; i >= 0; i--) {
                if (sessions[i]->film_id == f->id) {
                    sessions->RemoveAt(i);
                }
            }
            films->RemoveAt(index);
            SaveAll();
        }
    }

    void DeleteSessionAtIndex(int index) {
        if (index >= 0 && index < sessions->Count) {
            sessions->RemoveAt(index);
            SaveSessions();
        }
    }

    List<Session^>^ GetSessions() { return sessions; }

    // ========== ЗАПРОСЫ ==========

    List<String^>^ GetRepertoireByDate(int cinemaId, String^ date) {
        List<String^>^ res = gcnew List<String^>();
        res->Add(String::Format(L"{0,-12} {1,-8} {2,-30} {3,-8} {4,-6}",
            L"Дата", L"Время", L"Фильм", L"Цена", L"Своб.мест"));
        res->Add(String::Format(L"{0,-12} {1,-8} {2,-30} {3,-8} {4,-6}",
            L"------------", L"--------", L"------------------------------", L"--------", L"------"));

        for each(Session ^ s in sessions) {
            if (s->cinema_id == cinemaId && s->date == date) {
                String^ filmTitle = GetFilmTitleById(s->film_id);
                if (filmTitle->Length > 30) filmTitle = filmTitle->Substring(0, 27) + L"...";
                res->Add(String::Format(L"{0,-12} {1,-8} {2,-30} {3,-8} {4,-6}",
                    s->date, s->time, filmTitle, s->price, s->freeSeats()));
            }
        }
        if (res->Count == 2) res->Add(L"Нет сеансов на выбранную дату");
        return res;
    }

    int GetFreeSeats(int cinemaId, String^ date, String^ time) {
        for each(Session ^ s in sessions)
            if (s->cinema_id == cinemaId && s->date == date && s->time == time)
                return s->freeSeats();
        return -1;
    }

    int GetSoldTicketsForFilmPeriod(String^ filmTitle, String^ startDate, String^ endDate) {
        int fid = GetFilmId(filmTitle);
        if (fid == -1) return -1;
        int total = 0;
        DateTime start = DateTime::Parse(startDate);
        DateTime end = DateTime::Parse(endDate);
        for each(Session ^ s in sessions) {
            if (s->film_id == fid) {
                DateTime sessionDate = DateTime::Parse(s->date);
                if (sessionDate >= start && sessionDate <= end) {
                    total += s->sold_tickets;
                }
            }
        }
        return total;
    }

    void SaveReportToFile(String^ filename, List<String^>^ lines) {
        StreamWriter^ sw = gcnew StreamWriter(filename, false, System::Text::Encoding::UTF8);
        for each(String ^ line in lines) sw->WriteLine(line);
        sw->Close();
    }

    // ========== ЭЛЕМЕНТЫ УПРАВЛЕНИЯ ==========
    TabControl^ tabControl;
    TabPage^ tabAdmin;
    TabPage^ tabRepertoire;
    TabPage^ tabFreeSeats;
    TabPage^ tabSoldTickets;

    // Репертуар
    ComboBox^ cmbCinemaRep;
    DateTimePicker^ dtpRepDate;
    Button^ btnShowRep;
    RichTextBox^ txtRep;
    Button^ btnSaveRep;

    // Свободные места
    ComboBox^ cmbCinemaFree;
    DateTimePicker^ dtpFreeDate;
    DateTimePicker^ dtpFreeTime;
    Button^ btnShowFree;
    Label^ lblFree;
    Button^ btnSaveFree;

    // Продажи
    ComboBox^ cmbFilmSold;
    DateTimePicker^ dtpSoldStart;
    DateTimePicker^ dtpSoldEnd;
    Button^ btnShowSold;
    Label^ lblSold;
    Button^ btnSaveSold;

    // Администрирование
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

    void LoadCinemasGrid() {
        dgvCinemas->Rows->Clear();
        for each(Cinema ^ c in cinemas) {
            dgvCinemas->Rows->Add(c->id, c->name, c->category, c->capacity, c->halls, c->state);
        }
    }

    void LoadFilmsGrid() {
        dgvFilms->Rows->Clear();
        for each(Film ^ f in films) {
            dgvFilms->Rows->Add(f->id, f->title, f->director, f->genre, f->studio);
        }
    }

    void LoadSessionsGrid() {
        dgvSessions->Rows->Clear();
        for each(Session ^ s in sessions) {
            dgvSessions->Rows->Add(s->id,
                GetCinemaNameById(s->cinema_id),
                GetFilmTitleById(s->film_id),
                s->date, s->time, s->price, s->total_seats, s->sold_tickets, s->freeSeats());
        }
    }

    void RefreshAll() {
        LoadCinemasGrid();
        LoadFilmsGrid();
        LoadSessionsGrid();

        array<String^>^ cinemaNames = GetCinemaNames();
        cmbCinemaRep->Items->Clear();
        cmbCinemaFree->Items->Clear();
        for each(String ^ c in cinemaNames) {
            cmbCinemaRep->Items->Add(c);
            cmbCinemaFree->Items->Add(c);
        }
        if (cmbCinemaRep->Items->Count > 0) {
            cmbCinemaRep->SelectedIndex = 0;
            cmbCinemaFree->SelectedIndex = 0;
        }

        array<String^>^ filmTitles = GetFilmTitles();
        cmbFilmSold->Items->Clear();
        for each(String ^ f in filmTitles) cmbFilmSold->Items->Add(f);
        if (cmbFilmSold->Items->Count > 0) cmbFilmSold->SelectedIndex = 0;
    }

    // ========== ОБРАБОТЧИКИ КИНОТЕАТРОВ ==========

    void OnAddCinema(Object^ sender, EventArgs^ e) {
        Form^ form = gcnew Form();
        form->Text = L"Добавить кинотеатр";
        form->Size = Drawing::Size(400, 280);
        form->StartPosition = FormStartPosition::CenterParent;
        form->MaximizeBox = false;
        form->MinimizeBox = false;

        Label^ lblName = gcnew Label(); lblName->Text = L"Название:"; lblName->Location = Point(20, 20); lblName->Size = Drawing::Size(100, 25);
        TextBox^ txtName = gcnew TextBox(); txtName->Location = Point(130, 20); txtName->Size = Drawing::Size(220, 25);

        Label^ lblCategory = gcnew Label(); lblCategory->Text = L"Категория:"; lblCategory->Location = Point(20, 55); lblCategory->Size = Drawing::Size(100, 25);
        TextBox^ txtCategory = gcnew TextBox(); txtCategory->Location = Point(130, 55); txtCategory->Size = Drawing::Size(220, 25);

        Label^ lblCapacity = gcnew Label(); lblCapacity->Text = L"Вместимость:"; lblCapacity->Location = Point(20, 90); lblCapacity->Size = Drawing::Size(100, 25);
        NumericUpDown^ numCapacity = gcnew NumericUpDown(); numCapacity->Location = Point(130, 90); numCapacity->Size = Drawing::Size(100, 25); numCapacity->Minimum = 1; numCapacity->Maximum = 5000;

        Label^ lblHalls = gcnew Label(); lblHalls->Text = L"Кол-во залов:"; lblHalls->Location = Point(20, 125); lblHalls->Size = Drawing::Size(100, 25);
        NumericUpDown^ numHalls = gcnew NumericUpDown(); numHalls->Location = Point(130, 125); numHalls->Size = Drawing::Size(100, 25); numHalls->Minimum = 1; numHalls->Maximum = 20;

        Label^ lblState = gcnew Label(); lblState->Text = L"Состояние:"; lblState->Location = Point(20, 160); lblState->Size = Drawing::Size(100, 25);
        ComboBox^ cmbState = gcnew ComboBox(); cmbState->Location = Point(130, 160); cmbState->Size = Drawing::Size(120, 25); cmbState->DropDownStyle = ComboBoxStyle::DropDownList;
        cmbState->Items->Add(L"работает"); cmbState->Items->Add(L"ремонт"); cmbState->Items->Add(L"закрыт"); cmbState->SelectedIndex = 0;

        Button^ btnOk = gcnew Button(); btnOk->Text = L"OK"; btnOk->Location = Point(180, 200); btnOk->Size = Drawing::Size(80, 30);
        Button^ btnCancel = gcnew Button(); btnCancel->Text = L"Отмена"; btnCancel->Location = Point(270, 200); btnCancel->Size = Drawing::Size(80, 30);

        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        form->Controls->Add(lblName); form->Controls->Add(txtName);
        form->Controls->Add(lblCategory); form->Controls->Add(txtCategory);
        form->Controls->Add(lblCapacity); form->Controls->Add(numCapacity);
        form->Controls->Add(lblHalls); form->Controls->Add(numHalls);
        form->Controls->Add(lblState); form->Controls->Add(cmbState);
        form->Controls->Add(btnOk); form->Controls->Add(btnCancel);

        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK && txtName->Text->Length > 0) {
            AddCinema(txtName->Text, txtCategory->Text, (int)numCapacity->Value, (int)numHalls->Value, cmbState->SelectedItem->ToString());
            RefreshAll();
            MessageBox::Show(L"Кинотеатр добавлен!", L"Готово");
        }
    }

    void OnEditCinema(Object^ sender, EventArgs^ e) {
        if (dgvCinemas->SelectedRows->Count == 0) {
            MessageBox::Show(L"Выберите кинотеатр для редактирования!", L"Ошибка");
            return;
        }

        int index = dgvCinemas->SelectedRows[0]->Index;
        Cinema^ c = cinemas[index];

        Form^ form = gcnew Form();
        form->Text = L"Редактировать кинотеатр";
        form->Size = Drawing::Size(400, 280);
        form->StartPosition = FormStartPosition::CenterParent;
        form->MaximizeBox = false;
        form->MinimizeBox = false;

        Label^ lblName = gcnew Label(); lblName->Text = L"Название:"; lblName->Location = Point(20, 20); lblName->Size = Drawing::Size(100, 25);
        TextBox^ txtName = gcnew TextBox(); txtName->Location = Point(130, 20); txtName->Size = Drawing::Size(220, 25); txtName->Text = c->name;

        Label^ lblCategory = gcnew Label(); lblCategory->Text = L"Категория:"; lblCategory->Location = Point(20, 55); lblCategory->Size = Drawing::Size(100, 25);
        TextBox^ txtCategory = gcnew TextBox(); txtCategory->Location = Point(130, 55); txtCategory->Size = Drawing::Size(220, 25); txtCategory->Text = c->category;

        Label^ lblCapacity = gcnew Label(); lblCapacity->Text = L"Вместимость:"; lblCapacity->Location = Point(20, 90); lblCapacity->Size = Drawing::Size(100, 25);
        NumericUpDown^ numCapacity = gcnew NumericUpDown(); numCapacity->Location = Point(130, 90); numCapacity->Size = Drawing::Size(100, 25); numCapacity->Minimum = 1; numCapacity->Maximum = 5000; numCapacity->Value = (Decimal)c->capacity;

        Label^ lblHalls = gcnew Label(); lblHalls->Text = L"Кол-во залов:"; lblHalls->Location = Point(20, 125); lblHalls->Size = Drawing::Size(100, 25);
        NumericUpDown^ numHalls = gcnew NumericUpDown(); numHalls->Location = Point(130, 125); numHalls->Size = Drawing::Size(100, 25); numHalls->Minimum = 1; numHalls->Maximum = 20; numHalls->Value = (Decimal)c->halls;

        Label^ lblState = gcnew Label(); lblState->Text = L"Состояние:"; lblState->Location = Point(20, 160); lblState->Size = Drawing::Size(100, 25);
        ComboBox^ cmbState = gcnew ComboBox(); cmbState->Location = Point(130, 160); cmbState->Size = Drawing::Size(120, 25); cmbState->DropDownStyle = ComboBoxStyle::DropDownList;
        cmbState->Items->Add(L"работает"); cmbState->Items->Add(L"ремонт"); cmbState->Items->Add(L"закрыт"); cmbState->Text = c->state;

        Button^ btnOk = gcnew Button(); btnOk->Text = L"OK"; btnOk->Location = Point(180, 200); btnOk->Size = Drawing::Size(80, 30);
        Button^ btnCancel = gcnew Button(); btnCancel->Text = L"Отмена"; btnCancel->Location = Point(270, 200); btnCancel->Size = Drawing::Size(80, 30);

        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        form->Controls->Add(lblName); form->Controls->Add(txtName);
        form->Controls->Add(lblCategory); form->Controls->Add(txtCategory);
        form->Controls->Add(lblCapacity); form->Controls->Add(numCapacity);
        form->Controls->Add(lblHalls); form->Controls->Add(numHalls);
        form->Controls->Add(lblState); form->Controls->Add(cmbState);
        form->Controls->Add(btnOk); form->Controls->Add(btnCancel);

        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK && txtName->Text->Length > 0) {
            UpdateCinema(index, txtName->Text, txtCategory->Text, (int)numCapacity->Value, (int)numHalls->Value, cmbState->SelectedItem->ToString());
            RefreshAll();
            MessageBox::Show(L"Кинотеатр изменён!", L"Готово");
        }
    }

    void OnDeleteCinema(Object^ sender, EventArgs^ e) {
        if (dgvCinemas->SelectedRows->Count == 0) {
            MessageBox::Show(L"Выберите кинотеатр для удаления!", L"Ошибка");
            return;
        }
        int index = dgvCinemas->SelectedRows[0]->Index;
        Cinema^ c = cinemas[index];
        bool hasSessions = false;
        for each(Session ^ s in sessions) {
            if (s->cinema_id == c->id) {
                hasSessions = true;
                break;
            }
        }
        if (hasSessions) {
            if (MessageBox::Show(L"У этого кинотеатра есть сеансы! Удалить всё равно?",
                L"Предупреждение", MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) {
                return;
            }
        }
        DeleteCinemaAtIndex(index);
        RefreshAll();
        MessageBox::Show(L"Кинотеатр удалён!", L"Готово");
    }

    // ========== ОБРАБОТЧИКИ ФИЛЬМОВ ==========

    void OnAddFilm(Object^ sender, EventArgs^ e) {
        Form^ form = gcnew Form();
        form->Text = L"Добавить фильм";
        form->Size = Drawing::Size(450, 250);
        form->StartPosition = FormStartPosition::CenterParent;
        form->MaximizeBox = false;
        form->MinimizeBox = false;

        Label^ lblTitle = gcnew Label(); lblTitle->Text = L"Название:"; lblTitle->Location = Point(20, 20); lblTitle->Size = Drawing::Size(100, 25);
        TextBox^ txtTitle = gcnew TextBox(); txtTitle->Location = Point(130, 20); txtTitle->Size = Drawing::Size(270, 25);

        Label^ lblDirector = gcnew Label(); lblDirector->Text = L"Режиссёр:"; lblDirector->Location = Point(20, 55); lblDirector->Size = Drawing::Size(100, 25);
        TextBox^ txtDirector = gcnew TextBox(); txtDirector->Location = Point(130, 55); txtDirector->Size = Drawing::Size(270, 25);

        Label^ lblGenre = gcnew Label(); lblGenre->Text = L"Жанр:"; lblGenre->Location = Point(20, 90); lblGenre->Size = Drawing::Size(100, 25);
        TextBox^ txtGenre = gcnew TextBox(); txtGenre->Location = Point(130, 90); txtGenre->Size = Drawing::Size(270, 25);

        Label^ lblStudio = gcnew Label(); lblStudio->Text = L"Студия:"; lblStudio->Location = Point(20, 125); lblStudio->Size = Drawing::Size(100, 25);
        TextBox^ txtStudio = gcnew TextBox(); txtStudio->Location = Point(130, 125); txtStudio->Size = Drawing::Size(270, 25);

        Button^ btnOk = gcnew Button(); btnOk->Text = L"OK"; btnOk->Location = Point(230, 170); btnOk->Size = Drawing::Size(80, 30);
        Button^ btnCancel = gcnew Button(); btnCancel->Text = L"Отмена"; btnCancel->Location = Point(320, 170); btnCancel->Size = Drawing::Size(80, 30);

        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        form->Controls->Add(lblTitle); form->Controls->Add(txtTitle);
        form->Controls->Add(lblDirector); form->Controls->Add(txtDirector);
        form->Controls->Add(lblGenre); form->Controls->Add(txtGenre);
        form->Controls->Add(lblStudio); form->Controls->Add(txtStudio);
        form->Controls->Add(btnOk); form->Controls->Add(btnCancel);

        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK && txtTitle->Text->Length > 0) {
            AddFilm(txtTitle->Text, txtDirector->Text, txtGenre->Text, txtStudio->Text);
            RefreshAll();
            MessageBox::Show(L"Фильм добавлен!", L"Готово");
        }
    }

    void OnEditFilm(Object^ sender, EventArgs^ e) {
        if (dgvFilms->SelectedRows->Count == 0) {
            MessageBox::Show(L"Выберите фильм для редактирования!", L"Ошибка");
            return;
        }

        int index = dgvFilms->SelectedRows[0]->Index;
        Film^ f = films[index];

        Form^ form = gcnew Form();
        form->Text = L"Редактировать фильм";
        form->Size = Drawing::Size(450, 250);
        form->StartPosition = FormStartPosition::CenterParent;
        form->MaximizeBox = false;
        form->MinimizeBox = false;

        Label^ lblTitle = gcnew Label(); lblTitle->Text = L"Название:"; lblTitle->Location = Point(20, 20); lblTitle->Size = Drawing::Size(100, 25);
        TextBox^ txtTitle = gcnew TextBox(); txtTitle->Location = Point(130, 20); txtTitle->Size = Drawing::Size(270, 25); txtTitle->Text = f->title;

        Label^ lblDirector = gcnew Label(); lblDirector->Text = L"Режиссёр:"; lblDirector->Location = Point(20, 55); lblDirector->Size = Drawing::Size(100, 25);
        TextBox^ txtDirector = gcnew TextBox(); txtDirector->Location = Point(130, 55); txtDirector->Size = Drawing::Size(270, 25); txtDirector->Text = f->director;

        Label^ lblGenre = gcnew Label(); lblGenre->Text = L"Жанр:"; lblGenre->Location = Point(20, 90); lblGenre->Size = Drawing::Size(100, 25);
        TextBox^ txtGenre = gcnew TextBox(); txtGenre->Location = Point(130, 90); txtGenre->Size = Drawing::Size(270, 25); txtGenre->Text = f->genre;

        Label^ lblStudio = gcnew Label(); lblStudio->Text = L"Студия:"; lblStudio->Location = Point(20, 125); lblStudio->Size = Drawing::Size(100, 25);
        TextBox^ txtStudio = gcnew TextBox(); txtStudio->Location = Point(130, 125); txtStudio->Size = Drawing::Size(270, 25); txtStudio->Text = f->studio;

        Button^ btnOk = gcnew Button(); btnOk->Text = L"OK"; btnOk->Location = Point(230, 170); btnOk->Size = Drawing::Size(80, 30);
        Button^ btnCancel = gcnew Button(); btnCancel->Text = L"Отмена"; btnCancel->Location = Point(320, 170); btnCancel->Size = Drawing::Size(80, 30);

        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        form->Controls->Add(lblTitle); form->Controls->Add(txtTitle);
        form->Controls->Add(lblDirector); form->Controls->Add(txtDirector);
        form->Controls->Add(lblGenre); form->Controls->Add(txtGenre);
        form->Controls->Add(lblStudio); form->Controls->Add(txtStudio);
        form->Controls->Add(btnOk); form->Controls->Add(btnCancel);

        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK && txtTitle->Text->Length > 0) {
            UpdateFilm(index, txtTitle->Text, txtDirector->Text, txtGenre->Text, txtStudio->Text);
            RefreshAll();
            MessageBox::Show(L"Фильм изменён!", L"Готово");
        }
    }

    void OnDeleteFilm(Object^ sender, EventArgs^ e) {
        if (dgvFilms->SelectedRows->Count == 0) {
            MessageBox::Show(L"Выберите фильм для удаления!", L"Ошибка");
            return;
        }
        int index = dgvFilms->SelectedRows[0]->Index;
        Film^ f = films[index];
        bool hasSessions = false;
        for each(Session ^ s in sessions) {
            if (s->film_id == f->id) {
                hasSessions = true;
                break;
            }
        }
        if (hasSessions) {
            if (MessageBox::Show(L"У этого фильма есть сеансы! Удалить всё равно?",
                L"Предупреждение", MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes) {
                return;
            }
        }
        DeleteFilmAtIndex(index);
        RefreshAll();
        MessageBox::Show(L"Фильм удалён!", L"Готово");
    }

    // ========== ОБРАБОТЧИКИ СЕАНСОВ ==========

    void OnAddSession(Object^ sender, EventArgs^ e) {
        if (cinemas->Count == 0) {
            MessageBox::Show(L"Сначала добавьте кинотеатр!", L"Ошибка");
            return;
        }
        if (films->Count == 0) {
            MessageBox::Show(L"Сначала добавьте фильм!", L"Ошибка");
            return;
        }

        Form^ form = gcnew Form();
        form->Text = L"Добавить сеанс";
        form->Size = Drawing::Size(450, 450);
        form->StartPosition = FormStartPosition::CenterParent;
        form->MaximizeBox = false;
        form->MinimizeBox = false;

        int y = 10;

        Label^ lblCinema = gcnew Label(); lblCinema->Text = L"Кинотеатр:"; lblCinema->Location = Point(20, y); lblCinema->Size = Drawing::Size(100, 25);
        ComboBox^ cmbCinema = gcnew ComboBox(); cmbCinema->Location = Point(130, y); cmbCinema->Size = Drawing::Size(270, 25); cmbCinema->DropDownStyle = ComboBoxStyle::DropDownList;
        for each(Cinema ^ c in cinemas) cmbCinema->Items->Add(c->name);
        if (cmbCinema->Items->Count > 0) cmbCinema->SelectedIndex = 0;
        y += 40;

        Label^ lblFilm = gcnew Label(); lblFilm->Text = L"Фильм:"; lblFilm->Location = Point(20, y); lblFilm->Size = Drawing::Size(100, 25);
        ComboBox^ cmbFilm = gcnew ComboBox(); cmbFilm->Location = Point(130, y); cmbFilm->Size = Drawing::Size(270, 25); cmbFilm->DropDownStyle = ComboBoxStyle::DropDownList;
        for each(Film ^ f in films) cmbFilm->Items->Add(f->title);
        if (cmbFilm->Items->Count > 0) cmbFilm->SelectedIndex = 0;
        y += 40;

        Label^ lblDate = gcnew Label(); lblDate->Text = L"Дата:"; lblDate->Location = Point(20, y); lblDate->Size = Drawing::Size(100, 25);
        DateTimePicker^ dtpDate = gcnew DateTimePicker(); dtpDate->Location = Point(130, y); dtpDate->Size = Drawing::Size(150, 25); dtpDate->Format = DateTimePickerFormat::Short;
        y += 40;

        Label^ lblTime = gcnew Label(); lblTime->Text = L"Время:"; lblTime->Location = Point(20, y); lblTime->Size = Drawing::Size(100, 25);
        DateTimePicker^ dtpTime = gcnew DateTimePicker(); dtpTime->Location = Point(130, y); dtpTime->Size = Drawing::Size(120, 25); dtpTime->Format = DateTimePickerFormat::Time; dtpTime->ShowUpDown = true;
        y += 40;

        Label^ lblPrice = gcnew Label(); lblPrice->Text = L"Цена:"; lblPrice->Location = Point(20, y); lblPrice->Size = Drawing::Size(100, 25);
        NumericUpDown^ numPrice = gcnew NumericUpDown(); numPrice->Location = Point(130, y); numPrice->Size = Drawing::Size(120, 25); numPrice->Minimum = 0; numPrice->Maximum = 5000; numPrice->DecimalPlaces = 2; numPrice->Value = 350;
        y += 40;

        Label^ lblSeatsTotal = gcnew Label(); lblSeatsTotal->Text = L"Всего мест:"; lblSeatsTotal->Location = Point(20, y); lblSeatsTotal->Size = Drawing::Size(100, 25);
        NumericUpDown^ numTotalSeats = gcnew NumericUpDown(); numTotalSeats->Location = Point(130, y); numTotalSeats->Size = Drawing::Size(120, 25); numTotalSeats->Minimum = 1; numTotalSeats->Maximum = 500; numTotalSeats->Value = 250;
        y += 40;

        Label^ lblSold = gcnew Label(); lblSold->Text = L"Продано билетов:"; lblSold->Location = Point(20, y); lblSold->Size = Drawing::Size(100, 25);
        NumericUpDown^ numSold = gcnew NumericUpDown(); numSold->Location = Point(130, y); numSold->Size = Drawing::Size(120, 25); numSold->Minimum = 0; numSold->Maximum = 500; numSold->Value = 0;
        y += 50;

        Button^ btnOk = gcnew Button(); btnOk->Text = L"OK"; btnOk->Location = Point(180, y); btnOk->Size = Drawing::Size(80, 30);
        Button^ btnCancel = gcnew Button(); btnCancel->Text = L"Отмена"; btnCancel->Location = Point(270, y); btnCancel->Size = Drawing::Size(80, 30);
        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        form->Controls->Add(lblCinema); form->Controls->Add(cmbCinema);
        form->Controls->Add(lblFilm); form->Controls->Add(cmbFilm);
        form->Controls->Add(lblDate); form->Controls->Add(dtpDate);
        form->Controls->Add(lblTime); form->Controls->Add(dtpTime);
        form->Controls->Add(lblPrice); form->Controls->Add(numPrice);
        form->Controls->Add(lblSeatsTotal); form->Controls->Add(numTotalSeats);
        form->Controls->Add(lblSold); form->Controls->Add(numSold);
        form->Controls->Add(btnOk); form->Controls->Add(btnCancel);

        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            int cinemaId = GetCinemaId(cmbCinema->SelectedItem->ToString());
            int filmId = GetFilmId(cmbFilm->SelectedItem->ToString());
            if (cinemaId == -1 || filmId == -1) return;

            String^ dateStr = dtpDate->Value.ToString(L"yyyy-MM-dd");
            String^ timeStr = dtpTime->Value.ToString(L"HH:mm");

            AddSession(cinemaId, filmId, dateStr, timeStr,
                (double)numPrice->Value, (int)numTotalSeats->Value, (int)numSold->Value);
            RefreshAll();
            MessageBox::Show(L"Сеанс добавлен!", L"Готово");
        }
    }

    void OnEditSession(Object^ sender, EventArgs^ e) {
        if (dgvSessions->SelectedRows->Count == 0) {
            MessageBox::Show(L"Выберите сеанс для редактирования!", L"Ошибка");
            return;
        }

        int index = dgvSessions->SelectedRows[0]->Index;
        Session^ s = sessions[index];

        Form^ form = gcnew Form();
        form->Text = L"Редактировать сеанс";
        form->Size = Drawing::Size(450, 450);
        form->StartPosition = FormStartPosition::CenterParent;
        form->MaximizeBox = false;
        form->MinimizeBox = false;

        int y = 10;

        Label^ lblCinema = gcnew Label(); lblCinema->Text = L"Кинотеатр:"; lblCinema->Location = Point(20, y); lblCinema->Size = Drawing::Size(100, 25);
        ComboBox^ cmbCinema = gcnew ComboBox(); cmbCinema->Location = Point(130, y); cmbCinema->Size = Drawing::Size(270, 25); cmbCinema->DropDownStyle = ComboBoxStyle::DropDownList;
        for each(Cinema ^ c in cinemas) cmbCinema->Items->Add(c->name);
        cmbCinema->Text = GetCinemaNameById(s->cinema_id);
        y += 40;

        Label^ lblFilm = gcnew Label(); lblFilm->Text = L"Фильм:"; lblFilm->Location = Point(20, y); lblFilm->Size = Drawing::Size(100, 25);
        ComboBox^ cmbFilm = gcnew ComboBox(); cmbFilm->Location = Point(130, y); cmbFilm->Size = Drawing::Size(270, 25); cmbFilm->DropDownStyle = ComboBoxStyle::DropDownList;
        for each(Film ^ f in films) cmbFilm->Items->Add(f->title);
        cmbFilm->Text = GetFilmTitleById(s->film_id);
        y += 40;

        Label^ lblDate = gcnew Label(); lblDate->Text = L"Дата:"; lblDate->Location = Point(20, y); lblDate->Size = Drawing::Size(100, 25);
        DateTimePicker^ dtpDate = gcnew DateTimePicker(); dtpDate->Location = Point(130, y); dtpDate->Size = Drawing::Size(150, 25); dtpDate->Format = DateTimePickerFormat::Short;
        dtpDate->Value = DateTime::Parse(s->date);
        y += 40;

        Label^ lblTime = gcnew Label(); lblTime->Text = L"Время:"; lblTime->Location = Point(20, y); lblTime->Size = Drawing::Size(100, 25);
        DateTimePicker^ dtpTime = gcnew DateTimePicker(); dtpTime->Location = Point(130, y); dtpTime->Size = Drawing::Size(120, 25); dtpTime->Format = DateTimePickerFormat::Time; dtpTime->ShowUpDown = true;
        dtpTime->Value = DateTime::Parse(s->time);
        y += 40;

        Label^ lblPrice = gcnew Label(); lblPrice->Text = L"Цена:"; lblPrice->Location = Point(20, y); lblPrice->Size = Drawing::Size(100, 25);
        NumericUpDown^ numPrice = gcnew NumericUpDown(); numPrice->Location = Point(130, y); numPrice->Size = Drawing::Size(120, 25); numPrice->Minimum = 0; numPrice->Maximum = 5000; numPrice->DecimalPlaces = 2; numPrice->Value = (Decimal)s->price;
        y += 40;

        Label^ lblSeatsTotal = gcnew Label(); lblSeatsTotal->Text = L"Всего мест:"; lblSeatsTotal->Location = Point(20, y); lblSeatsTotal->Size = Drawing::Size(100, 25);
        NumericUpDown^ numTotalSeats = gcnew NumericUpDown(); numTotalSeats->Location = Point(130, y); numTotalSeats->Size = Drawing::Size(120, 25); numTotalSeats->Minimum = 1; numTotalSeats->Maximum = 500; numTotalSeats->Value = (Decimal)s->total_seats;
        y += 40;

        Label^ lblSold = gcnew Label(); lblSold->Text = L"Продано билетов:"; lblSold->Location = Point(20, y); lblSold->Size = Drawing::Size(100, 25);
        NumericUpDown^ numSold = gcnew NumericUpDown(); numSold->Location = Point(130, y); numSold->Size = Drawing::Size(120, 25); numSold->Minimum = 0; numSold->Maximum = 500; numSold->Value = (Decimal)s->sold_tickets;
        y += 50;

        Button^ btnOk = gcnew Button(); btnOk->Text = L"OK"; btnOk->Location = Point(180, y); btnOk->Size = Drawing::Size(80, 30);
        Button^ btnCancel = gcnew Button(); btnCancel->Text = L"Отмена"; btnCancel->Location = Point(270, y); btnCancel->Size = Drawing::Size(80, 30);
        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        form->Controls->Add(lblCinema); form->Controls->Add(cmbCinema);
        form->Controls->Add(lblFilm); form->Controls->Add(cmbFilm);
        form->Controls->Add(lblDate); form->Controls->Add(dtpDate);
        form->Controls->Add(lblTime); form->Controls->Add(dtpTime);
        form->Controls->Add(lblPrice); form->Controls->Add(numPrice);
        form->Controls->Add(lblSeatsTotal); form->Controls->Add(numTotalSeats);
        form->Controls->Add(lblSold); form->Controls->Add(numSold);
        form->Controls->Add(btnOk); form->Controls->Add(btnCancel);

        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            int cinemaId = GetCinemaId(cmbCinema->SelectedItem->ToString());
            int filmId = GetFilmId(cmbFilm->SelectedItem->ToString());
            if (cinemaId == -1 || filmId == -1) return;

            String^ dateStr = dtpDate->Value.ToString(L"yyyy-MM-dd");
            String^ timeStr = dtpTime->Value.ToString(L"HH:mm");

            UpdateSession(index, cinemaId, filmId, dateStr, timeStr,
                (double)numPrice->Value, (int)numTotalSeats->Value, (int)numSold->Value);
            RefreshAll();
            MessageBox::Show(L"Сеанс изменён!", L"Готово");
        }
    }

    void OnDeleteSession(Object^ sender, EventArgs^ e) {
        if (dgvSessions->SelectedRows->Count == 0) {
            MessageBox::Show(L"Выберите сеанс для удаления!", L"Ошибка");
            return;
        }
        int index = dgvSessions->SelectedRows[0]->Index;
        if (MessageBox::Show(L"Удалить выбранный сеанс?", L"Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
            DeleteSessionAtIndex(index);
            RefreshAll();
            MessageBox::Show(L"Сеанс удалён!", L"Готово");
        }
    }

    // ========== ОБРАБОТЧИКИ ОТЧЁТОВ ==========

    void OnShowRepertoire(Object^ sender, EventArgs^ e) {
        if (cmbCinemaRep->SelectedIndex < 0) return;
        int id = GetCinemaId(cmbCinemaRep->SelectedItem->ToString());
        String^ dateStr = dtpRepDate->Value.ToString(L"yyyy-MM-dd");
        List<String^>^ rep = GetRepertoireByDate(id, dateStr);
        txtRep->Clear();
        for each(String ^ line in rep) txtRep->AppendText(line + L"\n");
    }

    void OnSaveRepertoire(Object^ sender, EventArgs^ e) {
        SaveFileDialog^ dlg = gcnew SaveFileDialog();
        dlg->Filter = L"Текстовые файлы|*.txt";
        if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            List<String^>^ lines = gcnew List<String^>();
            lines->Add(L"=== РЕПЕРТУАР ===");
            lines->Add(L"Кинотеатр: " + cmbCinemaRep->SelectedItem->ToString());
            lines->Add(L"Дата: " + dtpRepDate->Value.ToString(L"yyyy-MM-dd"));
            lines->Add(L"");
            for each(String ^ line in txtRep->Lines) lines->Add(line);
            SaveReportToFile(dlg->FileName, lines);
            MessageBox::Show(L"Сохранено!");
        }
    }

    void OnShowFreeSeats(Object^ sender, EventArgs^ e) {
        if (cmbCinemaFree->SelectedIndex < 0) return;
        int id = GetCinemaId(cmbCinemaFree->SelectedItem->ToString());
        String^ dateStr = dtpFreeDate->Value.ToString(L"yyyy-MM-dd");
        String^ timeStr = dtpFreeTime->Value.ToString(L"HH:mm");
        int free = GetFreeSeats(id, dateStr, timeStr);
        if (free == -1) {
            lblFree->Text = L"❌ Сеанс не найден";
            lblFree->ForeColor = Color::Red;
        }
        else {
            lblFree->Text = L"✅ Свободных мест: " + free;
            lblFree->ForeColor = Color::Green;
        }
    }

    void OnSaveFreeResult(Object^ sender, EventArgs^ e) {
        SaveFileDialog^ dlg = gcnew SaveFileDialog();
        dlg->Filter = L"Текстовые файлы|*.txt";
        if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            List<String^>^ lines = gcnew List<String^>();
            lines->Add(L"=== СВОБОДНЫЕ МЕСТА ===");
            lines->Add(lblFree->Text);
            SaveReportToFile(dlg->FileName, lines);
            MessageBox::Show(L"Сохранено!");
        }
    }

    void OnShowSoldTickets(Object^ sender, EventArgs^ e) {
        if (cmbFilmSold->SelectedIndex < 0) return;
        String^ filmTitle = cmbFilmSold->SelectedItem->ToString();
        String^ startDate = dtpSoldStart->Value.ToString(L"yyyy-MM-dd");
        String^ endDate = dtpSoldEnd->Value.ToString(L"yyyy-MM-dd");
        int sold = GetSoldTicketsForFilmPeriod(filmTitle, startDate, endDate);
        lblSold->Text = L"🎫 Продано билетов за период: " + sold;
    }

    void OnSaveSoldReport(Object^ sender, EventArgs^ e) {
        SaveFileDialog^ dlg = gcnew SaveFileDialog();
        dlg->Filter = L"Текстовые файлы|*.txt";
        if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            List<String^>^ lines = gcnew List<String^>();
            lines->Add(L"=== ОТЧЁТ О ПРОДАННЫХ БИЛЕТАХ ===");
            lines->Add(L"Фильм: " + cmbFilmSold->SelectedItem->ToString());
            lines->Add(L"Период: " + dtpSoldStart->Value.ToString(L"yyyy-MM-dd") + " - " + dtpSoldEnd->Value.ToString(L"yyyy-MM-dd"));
            lines->Add(lblSold->Text);
            SaveReportToFile(dlg->FileName, lines);
            MessageBox::Show(L"Сохранено!");
        }
    }

    void OnExit(Object^ sender, EventArgs^ e) {
        if (MessageBox::Show(L"Вы уверены, что хотите выйти?",
            L"Подтверждение выхода",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
            Application::Exit();
        }
    }

    void InitializeComponent() {
        this->Icon = gcnew System::Drawing::Icon("iconka_1.ico");
        this->Text = L"KinoDOGandFOX - Справочная служба";
        this->Size = Drawing::Size(1400, 900);
        this->StartPosition = FormStartPosition::CenterScreen;

        // Иконка
        try {
            this->Icon = gcnew System::Drawing::Icon("iconka.ico");
        }
        catch (...) {}

        tabControl = gcnew TabControl();
        tabControl->Dock = DockStyle::Fill;

        // Кнопка выхода
        btnExit = gcnew Button();
        btnExit->Text = L"❌ Выход";
        btnExit->Size = Drawing::Size(100, 35);
        btnExit->Location = Point(this->ClientSize.Width - 110, this->ClientSize.Height - 50);
        btnExit->Anchor = AnchorStyles::Bottom | AnchorStyles::Right;
        btnExit->BackColor = Color::IndianRed;
        btnExit->ForeColor = Color::White;
        btnExit->FlatStyle = FlatStyle::Popup;
        btnExit->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnExit);
        this->Controls->Add(btnExit);
        this->Resize += gcnew EventHandler(this, &KinoDOGandFOX::OnFormResize);

        // ========== АДМИНИСТРИРОВАНИЕ ==========
        tabAdmin = gcnew TabPage();
        tabAdmin->Text = L"📋 Администрирование";

        // Кинотеатры
        Label^ lblCinemasTitle = gcnew Label(); lblCinemasTitle->Text = L"Кинотеатры:"; lblCinemasTitle->Location = Point(10, 10); lblCinemasTitle->Size = Drawing::Size(100, 25);
        lblCinemasTitle->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold);

        dgvCinemas = gcnew DataGridView();
        dgvCinemas->Location = Point(10, 40);
        dgvCinemas->Size = Drawing::Size(1360, 130);
        dgvCinemas->AllowUserToAddRows = false;
        dgvCinemas->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        dgvCinemas->ReadOnly = true;
        dgvCinemas->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        dgvCinemas->Columns->Add(L"id", L"ID");
        dgvCinemas->Columns->Add(L"name", L"Название");
        dgvCinemas->Columns->Add(L"category", L"Категория");
        dgvCinemas->Columns->Add(L"capacity", L"Вместимость");
        dgvCinemas->Columns->Add(L"halls", L"Залы");
        dgvCinemas->Columns->Add(L"state", L"Состояние");

        btnAddCinema = gcnew Button(); btnAddCinema->Text = L"➕ Добавить"; btnAddCinema->Location = Point(10, 180); btnAddCinema->Size = Drawing::Size(100, 30);
        btnAddCinema->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnAddCinema);

        btnEditCinema = gcnew Button(); btnEditCinema->Text = L"✏️ Изменить"; btnEditCinema->Location = Point(120, 180); btnEditCinema->Size = Drawing::Size(100, 30);
        btnEditCinema->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnEditCinema);

        btnDeleteCinema = gcnew Button(); btnDeleteCinema->Text = L"🗑 Удалить"; btnDeleteCinema->Location = Point(230, 180); btnDeleteCinema->Size = Drawing::Size(100, 30);
        btnDeleteCinema->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnDeleteCinema);

        // Фильмы
        Label^ lblFilmsTitle = gcnew Label(); lblFilmsTitle->Text = L"Фильмы:"; lblFilmsTitle->Location = Point(10, 230); lblFilmsTitle->Size = Drawing::Size(100, 25);
        lblFilmsTitle->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold);

        dgvFilms = gcnew DataGridView();
        dgvFilms->Location = Point(10, 260);
        dgvFilms->Size = Drawing::Size(1360, 130);
        dgvFilms->AllowUserToAddRows = false;
        dgvFilms->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        dgvFilms->ReadOnly = true;
        dgvFilms->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        dgvFilms->Columns->Add(L"id", L"ID");
        dgvFilms->Columns->Add(L"title", L"Название");
        dgvFilms->Columns->Add(L"director", L"Режиссёр");
        dgvFilms->Columns->Add(L"genre", L"Жанр");
        dgvFilms->Columns->Add(L"studio", L"Студия");

        btnAddFilm = gcnew Button(); btnAddFilm->Text = L"➕ Добавить"; btnAddFilm->Location = Point(10, 400); btnAddFilm->Size = Drawing::Size(100, 30);
        btnAddFilm->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnAddFilm);

        btnEditFilm = gcnew Button(); btnEditFilm->Text = L"✏️ Изменить"; btnEditFilm->Location = Point(120, 400); btnEditFilm->Size = Drawing::Size(100, 30);
        btnEditFilm->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnEditFilm);

        btnDeleteFilm = gcnew Button(); btnDeleteFilm->Text = L"🗑 Удалить"; btnDeleteFilm->Location = Point(230, 400); btnDeleteFilm->Size = Drawing::Size(100, 30);
        btnDeleteFilm->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnDeleteFilm);

        // Сеансы
        Label^ lblSessionsTitle = gcnew Label(); lblSessionsTitle->Text = L"Сеансы:"; lblSessionsTitle->Location = Point(10, 450); lblSessionsTitle->Size = Drawing::Size(100, 25);
        lblSessionsTitle->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold);

        dgvSessions = gcnew DataGridView();
        dgvSessions->Location = Point(10, 480);
        dgvSessions->Size = Drawing::Size(1360, 150);
        dgvSessions->AllowUserToAddRows = false;
        dgvSessions->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        dgvSessions->ReadOnly = true;
        dgvSessions->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        dgvSessions->Columns->Add(L"id", L"ID");
        dgvSessions->Columns->Add(L"cinema", L"Кинотеатр");
        dgvSessions->Columns->Add(L"film", L"Фильм");
        dgvSessions->Columns->Add(L"date", L"Дата");
        dgvSessions->Columns->Add(L"time", L"Время");
        dgvSessions->Columns->Add(L"price", L"Цена");
        dgvSessions->Columns->Add(L"total", L"Всего мест");
        dgvSessions->Columns->Add(L"sold", L"Продано");
        dgvSessions->Columns->Add(L"free", L"Свободно");

        btnAddSession = gcnew Button(); btnAddSession->Text = L"➕ Добавить"; btnAddSession->Location = Point(10, 640); btnAddSession->Size = Drawing::Size(100, 30);
        btnAddSession->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnAddSession);

        btnEditSession = gcnew Button(); btnEditSession->Text = L"✏️ Изменить"; btnEditSession->Location = Point(120, 640); btnEditSession->Size = Drawing::Size(100, 30);
        btnEditSession->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnEditSession);

        btnDeleteSession = gcnew Button(); btnDeleteSession->Text = L"🗑 Удалить"; btnDeleteSession->Location = Point(230, 640); btnDeleteSession->Size = Drawing::Size(100, 30);
        btnDeleteSession->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnDeleteSession);

        tabAdmin->Controls->Add(lblCinemasTitle);
        tabAdmin->Controls->Add(dgvCinemas);
        tabAdmin->Controls->Add(btnAddCinema);
        tabAdmin->Controls->Add(btnEditCinema);
        tabAdmin->Controls->Add(btnDeleteCinema);
        tabAdmin->Controls->Add(lblFilmsTitle);
        tabAdmin->Controls->Add(dgvFilms);
        tabAdmin->Controls->Add(btnAddFilm);
        tabAdmin->Controls->Add(btnEditFilm);
        tabAdmin->Controls->Add(btnDeleteFilm);
        tabAdmin->Controls->Add(lblSessionsTitle);
        tabAdmin->Controls->Add(dgvSessions);
        tabAdmin->Controls->Add(btnAddSession);
        tabAdmin->Controls->Add(btnEditSession);
        tabAdmin->Controls->Add(btnDeleteSession);

        // ========== РЕПЕРТУАР ==========
        tabRepertoire = gcnew TabPage();
        tabRepertoire->Text = L"📋 Репертуар";

        cmbCinemaRep = gcnew ComboBox();
        cmbCinemaRep->Location = Point(10, 15);
        cmbCinemaRep->Size = Drawing::Size(220, 25);
        cmbCinemaRep->DropDownStyle = ComboBoxStyle::DropDownList;

        Label^ lblRepDate = gcnew Label();
        lblRepDate->Text = L"Дата:";
        lblRepDate->Location = Point(240, 17);
        lblRepDate->Size = Drawing::Size(50, 25);

        dtpRepDate = gcnew DateTimePicker();
        dtpRepDate->Location = Point(290, 15);
        dtpRepDate->Size = Drawing::Size(130, 25);
        dtpRepDate->Format = DateTimePickerFormat::Short;

        btnShowRep = gcnew Button();
        btnShowRep->Text = L"Показать";
        btnShowRep->Location = Point(430, 13);
        btnShowRep->Size = Drawing::Size(100, 30);
        btnShowRep->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnShowRepertoire);

        txtRep = gcnew RichTextBox();
        txtRep->Location = Point(10, 55);
        txtRep->Size = Drawing::Size(1360, 710);
        txtRep->Font = gcnew System::Drawing::Font(L"Consolas", 10);
        txtRep->ReadOnly = true;

        btnSaveRep = gcnew Button();
        btnSaveRep->Text = L"💾 Сохранить в файл";
        btnSaveRep->Location = Point(1250, 13);
        btnSaveRep->Size = Drawing::Size(120, 30);
        btnSaveRep->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnSaveRepertoire);

        tabRepertoire->Controls->Add(cmbCinemaRep);
        tabRepertoire->Controls->Add(lblRepDate);
        tabRepertoire->Controls->Add(dtpRepDate);
        tabRepertoire->Controls->Add(btnShowRep);
        tabRepertoire->Controls->Add(txtRep);
        tabRepertoire->Controls->Add(btnSaveRep);

        // ========== СВОБОДНЫЕ МЕСТА ==========
        tabFreeSeats = gcnew TabPage();
        tabFreeSeats->Text = L"🔍 Свободные места";

        cmbCinemaFree = gcnew ComboBox();
        cmbCinemaFree->Location = Point(10, 15);
        cmbCinemaFree->Size = Drawing::Size(220, 25);
        cmbCinemaFree->DropDownStyle = ComboBoxStyle::DropDownList;

        Label^ lblFreeDate = gcnew Label();
        lblFreeDate->Text = L"Дата:";
        lblFreeDate->Location = Point(240, 17);
        lblFreeDate->Size = Drawing::Size(50, 25);

        dtpFreeDate = gcnew DateTimePicker();
        dtpFreeDate->Location = Point(290, 15);
        dtpFreeDate->Size = Drawing::Size(130, 25);
        dtpFreeDate->Format = DateTimePickerFormat::Short;

        Label^ lblFreeTime = gcnew Label();
        lblFreeTime->Text = L"Время:";
        lblFreeTime->Location = Point(430, 17);
        lblFreeTime->Size = Drawing::Size(50, 25);

        dtpFreeTime = gcnew DateTimePicker();
        dtpFreeTime->Location = Point(480, 15);
        dtpFreeTime->Size = Drawing::Size(100, 25);
        dtpFreeTime->Format = DateTimePickerFormat::Time;
        dtpFreeTime->ShowUpDown = true;

        btnShowFree = gcnew Button();
        btnShowFree->Text = L"Узнать";
        btnShowFree->Location = Point(590, 13);
        btnShowFree->Size = Drawing::Size(100, 30);
        btnShowFree->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnShowFreeSeats);

        lblFree = gcnew Label();
        lblFree->Location = Point(10, 60);
        lblFree->Size = Drawing::Size(600, 40);
        lblFree->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold);

        btnSaveFree = gcnew Button();
        btnSaveFree->Text = L"💾 Сохранить результат";
        btnSaveFree->Location = Point(700, 13);
        btnSaveFree->Size = Drawing::Size(130, 30);
        btnSaveFree->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnSaveFreeResult);

        tabFreeSeats->Controls->Add(cmbCinemaFree);
        tabFreeSeats->Controls->Add(lblFreeDate);
        tabFreeSeats->Controls->Add(dtpFreeDate);
        tabFreeSeats->Controls->Add(lblFreeTime);
        tabFreeSeats->Controls->Add(dtpFreeTime);
        tabFreeSeats->Controls->Add(btnShowFree);
        tabFreeSeats->Controls->Add(lblFree);
        tabFreeSeats->Controls->Add(btnSaveFree);

        // ========== ПРОДАЖИ ==========
        tabSoldTickets = gcnew TabPage();
        tabSoldTickets->Text = L"$ Продажи по фильму";

        cmbFilmSold = gcnew ComboBox();
        cmbFilmSold->Location = Point(10, 15);
        cmbFilmSold->Size = Drawing::Size(250, 25);
        cmbFilmSold->DropDownStyle = ComboBoxStyle::DropDownList;

        Label^ lblFrom = gcnew Label();
        lblFrom->Text = L"с:";
        lblFrom->Location = Point(270, 17);
        lblFrom->Size = Drawing::Size(30, 25);

        dtpSoldStart = gcnew DateTimePicker();
        dtpSoldStart->Location = Point(300, 15);
        dtpSoldStart->Size = Drawing::Size(110, 25);
        dtpSoldStart->Format = DateTimePickerFormat::Short;

        Label^ lblTo = gcnew Label();
        lblTo->Text = L"по:";
        lblTo->Location = Point(420, 17);
        lblTo->Size = Drawing::Size(30, 25);

        dtpSoldEnd = gcnew DateTimePicker();
        dtpSoldEnd->Location = Point(450, 15);
        dtpSoldEnd->Size = Drawing::Size(110, 25);
        dtpSoldEnd->Format = DateTimePickerFormat::Short;
        dtpSoldEnd->Value = DateTime::Now;

        btnShowSold = gcnew Button();
        btnShowSold->Text = L"Показать";
        btnShowSold->Location = Point(570, 13);
        btnShowSold->Size = Drawing::Size(100, 30);
        btnShowSold->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnShowSoldTickets);

        lblSold = gcnew Label();
        lblSold->Location = Point(10, 60);
        lblSold->Size = Drawing::Size(700, 40);
        lblSold->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold);

        btnSaveSold = gcnew Button();
        btnSaveSold->Text = L"💾 Сохранить отчёт";
        btnSaveSold->Location = Point(10, 110);
        btnSaveSold->Size = Drawing::Size(150, 30);
        btnSaveSold->Click += gcnew EventHandler(this, &KinoDOGandFOX::OnSaveSoldReport);

        tabSoldTickets->Controls->Add(cmbFilmSold);
        tabSoldTickets->Controls->Add(lblFrom);
        tabSoldTickets->Controls->Add(dtpSoldStart);
        tabSoldTickets->Controls->Add(lblTo);
        tabSoldTickets->Controls->Add(dtpSoldEnd);
        tabSoldTickets->Controls->Add(btnShowSold);
        tabSoldTickets->Controls->Add(lblSold);
        tabSoldTickets->Controls->Add(btnSaveSold);

        tabControl->TabPages->Add(tabAdmin);
        tabControl->TabPages->Add(tabRepertoire);
        tabControl->TabPages->Add(tabFreeSeats);
        tabControl->TabPages->Add(tabSoldTickets);

        this->Controls->Add(tabControl);

        LoadCinemasGrid();
        LoadFilmsGrid();
        LoadSessionsGrid();
        RefreshAll();
    }

    void OnFormResize(Object^ sender, EventArgs^ e) {
        if (btnExit != nullptr) {
            btnExit->Location = Point(this->ClientSize.Width - btnExit->Width - 15,
                this->ClientSize.Height - btnExit->Height - 15);
        }
    }
};