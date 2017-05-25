#include "settingwnd.h"
#include "ui_settingwnd.h"

#include "SettingVar.h"
#include "rttr\type.h"
#include "EditorUtility.h"
#include "WellPlayMain.h"

using namespace rttr;
using namespace std;
#pragma comment(lib,"rttr_core_d.lib")

SettingVar<float> a(L"����/������", 0.5f, 0.0f, 2.0f);

SettingWnd::SettingWnd(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SettingWnd)
{
    ui->setupUi(this);

	//�����ж��̵߳�����
	auto lk = Interrupt();
	const auto& list = GetSettingList();
	QList<QTreeWidgetItem*> settingtree;
	for (auto& item : list)
	{
		QTreeWidgetItem* current = nullptr;
		auto path = item->GetPath();
		assert(path.size() != 0);
		while (true)
		{
			auto index = path.find('/');
			if (index == wstring::npos)
			{
				QTreeWidgetItem* newitem = new QTreeWidgetItem();
				newitem->setText(0, MakeQString(path));

				auto typestring = type::get(*item).get_name();
				auto controlltype = typestring + "set";
				type class_type = type::get_by_name(controlltype.data());
				if (class_type)
				{
					variant wrapper =class_type.create({ static_cast<QWidget*>(newitem->treeWidget()),static_cast<SettingBase*>(item)});
					//����ɾ����ֻ��wrapper������Ŀؼ����������νṹ��
					if (wrapper.get_type().is_pointer())
						class_type.destroy(wrapper);
				}
				else
				{
					QString data = QString::fromStdWString(L"�Ҳ�����Ӧ�ؼ���ʵ�֣�����Ϊ��");
					data += QString::fromStdString(typestring);
					Utility::Log(data, 2);
				}
				if (current == nullptr)
					settingtree.push_back(newitem);
				else
					current->addChild(newitem);
				break;
			}
			else
			{
				auto nodename = std::wstring(path, 0, index);
				assert(path.size() > index);
				path = std::wstring(path, index + 1);

				QTreeWidgetItem* newitem = new QTreeWidgetItem();
				newitem->setText(0, MakeQString(nodename));

				if (current == nullptr)
					settingtree.push_back(newitem);
				else
					current->addChild(newitem);
				current = newitem;
			}
		}
	}

	ui->treeWidget->addTopLevelItems(settingtree);
}

SettingWnd::~SettingWnd()
{
    delete ui;
}
