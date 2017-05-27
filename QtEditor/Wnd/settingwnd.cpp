#include "settingwnd.h"
#include "ui_settingwnd.h"
#include "SettingWrapper.hpp"
#include "rttr\type.h"
#include "EditorUtility.h"
#include "WellPlayMain.h"

using namespace rttr;
using namespace std;
#pragma comment(lib,"rttr_core_d.lib")

SettingVar<float> a(L"����/������", 0.5f, 0.0f, 2.0f);

SettingWnd::SettingWnd(QWidget *parent) :
    QDockWidget(parent),
	m_delegate(new SettingDelegate),
	m_model(new SettingModel),
    ui(new Ui::SettingWnd)
{
    ui->setupUi(this);

	ui->treeView->setModel(m_model);

	m_model->InitSetting();

	ui->treeView->setItemDelegate(m_delegate);
	ui->treeView->setHeaderHidden(false);
}

SettingWnd::~SettingWnd()
{
	delete m_delegate;
	delete m_model;
    delete ui;
}

QWidget * SettingDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (index.column() == 0)
		return nullptr;
	auto data = index.data(Qt::UserRole);
	if (!data.isValid())
		return nullptr;
	SettingBase* item = data.value<SettingBase*>();

	//�����ж��߳�����
	auto lk = Interrupt();

	auto typestring = type::get(*item).get_name();
	//�������set����Ϊ�������Ʋ����ظ�,���Կ��Լ���һ����
	auto controlltype = typestring + "set";
	type class_type = type::get_by_name(controlltype.data());
	if (class_type)
	{
		variant wrapper = class_type.create({ parent,item });
		//����ɾ����ֻ��wrapper������Ŀؼ����������νṹ��
		if (wrapper.get_type().is_pointer())
			class_type.destroy(wrapper);

		auto hehe = parent->findChild<SettingWidget*>();
		return static_cast<QWidget*>(hehe);
	}
	else
	{
		QString data = QString::fromStdWString(L"�Ҳ�����Ӧ�ؼ���ʵ�֣�����Ϊ��");
		data += QString::fromStdString(typestring);
		Utility::Log(data, 2);
		return nullptr;
	}
}

void SettingModel::InitSetting()
{
	qRegisterMetaType<SettingBase*>("SettingBase*");
	setHorizontalHeaderLabels(QStringList{ QString::fromStdWString(L"������") ,QString::fromStdWString(L"����ֵ") });
	const auto& list = GetSettingList();
	AddSettingItem(list);
}

void SettingModel::AddSettingItem(const vector<SettingBase*>& list)
{
	//ע�����ﲻҪ������item����������߳�����
	for (auto& item : list)
	{
		QStandardItem* current = nullptr;
		auto path = item->GetPath();
		assert(path.size() != 0);
		vector<QString> fullpath;
		size_t index = 0;
		while (true)
		{
			auto temp = index;
			index = path.find('/', temp);
			if (index != wstring::npos)
			{
				fullpath.push_back(QString::fromStdWString(wstring(path, temp, index - temp)));
				index++;
			}
			else
			{
				fullpath.push_back(QString::fromStdWString(wstring(path, temp, fullpath.size() - temp)));
				break;
			}
		}
		QStandardItem* parent = invisibleRootItem();
		for (size_t i = 0;i < fullpath.size();i++)
		{
			QString& itemname = fullpath[i];
			int j;
			for (j = 0;j < parent->rowCount();j++)
			{
				if (parent->child(j)->text() == itemname)
				{
					break;
				}
			}
			if (j == parent->rowCount())
			{
				QStandardItem* newitem = new QStandardItem();
				newitem->setText(itemname);
				parent->appendRow(newitem);

				if (i == fullpath.size() - 1)
				{
					assert(parent->child(newitem->row(), 1) == nullptr);
					QStandardItem* data = new QStandardItem();
					parent->setChild(newitem->row(), 1, data);
					parent = data;
				}
				else
					parent = newitem;
			}
			else
				parent = parent->child(j);
		    
			if (i == fullpath.size() - 1)
			{
				QVariant data;
				data.setValue<SettingBase*>(item);
				parent->setData(data, Qt::UserRole);
			}
		}
	}
}
