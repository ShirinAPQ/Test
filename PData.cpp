#include <iostream>
#include <string>
#include <vector>
// 仅保留 Connector/C++ 核心头文件，无任何 C API 痕迹
#include <mysql/jdbc.h>

// 命名空间简化代码，适配 Connector/C++
using namespace std;
using namespace sql;
using namespace sql::mysql;

// 连接信息 - 请根据你的 MySQL 实际配置修改
const char* HOST = "localhost";
const char* USER = "root";
const char* PASSWORD = "123456";
const char* DATABASE = "PData";
const unsigned int PORT = 3306;

// 人员信息结构体（保持原有功能不变）
struct Employee {
    int id;
    string name;
    int positionId;
    string positionName;
};

// 职务信息结构体（保持原有功能不变）
struct Position {
    int id;
    string name;
};

// 获取 Connector/C++ 数据库连接（9.6 版本兼容，无 C API 函数）
unique_ptr<Connection> getMySQLConnection() {
    try {
        // 1. 获取 MySQL 驱动实例（Connector/C++ 标准接口）
        MySQL_Driver* driver = get_mysql_driver_instance();
        if (!driver) {
            cerr << "获取 MySQL 驱动失败！" << endl;
            return nullptr;
        }

        // 2. 建立数据库连接
        unique_ptr<Connection> conn(
            driver->connect(HOST, USER, PASSWORD)
        );
        if (!conn) {
            cerr << "数据库连接失败！" << endl;
            return nullptr;
        }

        // 3. 选择目标数据库，设置字符集避免中文乱码（适配 9.6 版本）
        conn->setSchema(DATABASE);
        unique_ptr<Statement> stmt(conn->createStatement());
        stmt->execute("SET NAMES utf8mb4");

        return conn;
    } catch (SQLException& e) {
        // Connector/C++ 异常处理，报错信息直观
        cerr << "数据库连接异常：" << e.what() << endl;
        cerr << "错误代码：" << e.getErrorCode() << endl;
        return nullptr;
    }
}

// 添加职务（Connector/C++ 实现，使用 PreparedStatement 防 SQL 注入）
bool addPosition(Connection* conn, const string& positionName) {
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return false;
    }

    try {
        string sql = "INSERT INTO positions (position_name) VALUES (?)";
        unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(sql));
        pstmt->setString(1, positionName);
        pstmt->executeUpdate(); // 增删改操作使用 executeUpdate()

        cout << "职务添加成功：" << positionName << endl;
        return true;
    } catch (SQLException& e) {
        cerr << "添加职务失败：" << e.what() << endl;
        return false;
    }
}

// 获取所有职务（Connector/C++ 实现，遍历 ResultSet 结果集）
vector<Position> getAllPositions(Connection* conn) {
    vector<Position> positions;
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return positions;
    }

    try {
        string sql = "SELECT position_id, position_name FROM positions";
        unique_ptr<Statement> stmt(conn->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery(sql)); // 查询操作使用 executeQuery()

        // 遍历结果集，适配 Connector/C++ 结果集处理逻辑
        while (res->next()) {
            Position pos;
            pos.id = res->getInt("position_id");
            pos.name = res->getString("position_name");
            positions.push_back(pos);
        }
    } catch (SQLException& e) {
        cerr << "获取职务列表失败：" << e.what() << endl;
    }

    return positions;
}

// 添加人员（Connector/C++ 实现，保持原有功能）
bool addEmployee(Connection* conn, const string& employeeName, int positionId) {
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return false;
    }

    try {
        string sql = "INSERT INTO employees (employee_name, position_id) VALUES (?, ?)";
        unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(sql));
        pstmt->setString(1, employeeName);
        pstmt->setInt(2, positionId);
        pstmt->executeUpdate();

        cout << "人员添加成功：" << employeeName << endl;
        return true;
    } catch (SQLException& e) {
        cerr << "添加人员失败：" << e.what() << endl;
        return false;
    }
}

// 更新人员信息（Connector/C++ 实现，保持原有功能）
bool updateEmployee(Connection* conn, int employeeId, const string& newName, int newPositionId) {
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return false;
    }

    try {
        string sql = "UPDATE employees SET employee_name = ?, position_id = ? WHERE employee_id = ?";
        unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(sql));
        pstmt->setString(1, newName);
        pstmt->setInt(2, newPositionId);
        pstmt->setInt(3, employeeId);
        pstmt->executeUpdate();

        cout << "人员信息更新成功，ID：" << employeeId << endl;
        return true;
    } catch (SQLException& e) {
        cerr << "更新人员信息失败：" << e.what() << endl;
        return false;
    }
}

// 获取所有人员及其职务（Connector/C++ 实现，保持原有功能）
vector<Employee> getAllEmployees(Connection* conn) {
    vector<Employee> employees;
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return employees;
    }

    try {
        string sql = "SELECT e.employee_id, e.employee_name, e.position_id, p.position_name "
                     "FROM employees e JOIN positions p ON e.position_id = p.position_id";
        unique_ptr<Statement> stmt(conn->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery(sql));

        while (res->next()) {
            Employee emp;
            emp.id = res->getInt("employee_id");
            emp.name = res->getString("employee_name");
            emp.positionId = res->getInt("position_id");
            emp.positionName = res->getString("position_name");
            employees.push_back(emp);
        }
    } catch (SQLException& e) {
        cerr << "查询人员列表失败：" << e.what() << endl;
    }

    return employees;
}

// 根据 ID 查询人员（Connector/C++ 实现，保持原有功能）
Employee getEmployeeById(Connection* conn, int employeeId) {
    Employee emp = {0, "", 0, ""};
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return emp;
    }

    try {
        string sql = "SELECT e.employee_id, e.employee_name, e.position_id, p.position_name "
                     "FROM employees e JOIN positions p ON e.position_id = p.position_id "
                     "WHERE e.employee_id = ?";
        unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(sql));
        pstmt->setInt(1, employeeId);
        unique_ptr<ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            emp.id = res->getInt("employee_id");
            emp.name = res->getString("employee_name");
            emp.positionId = res->getInt("position_id");
            emp.positionName = res->getString("position_name");
        }
    } catch (SQLException& e) {
        cerr << "查询人员失败：" << e.what() << endl;
    }

    return emp;
}

// 打印人员列表（保持原有格式不变）
void printEmployees(const vector<Employee>& employees) {
    cout << "\n========= 人员列表 =========" << endl;
    cout << "ID | 姓名 | 职务ID | 职务名称" << endl;
    cout << "---------------------------" << endl;
    for (const auto& emp : employees) {
        cout << emp.id << " | " << emp.name << " | " << emp.positionId << " | " << emp.positionName << endl;
    }
    cout << "===========================" << endl;
}

// 打印职务列表（保持原有格式不变）
void printPositions(const vector<Position>& positions) {
    cout << "\n========= 职务列表 =========" << endl;
    cout << "ID | 职务名称" << endl;
    cout << "------------" << endl;
    for (const auto& pos : positions) {
        cout << pos.id << " | " << pos.name << endl;
    }
    cout << "===========================" << endl;
}

int main() {
    cout << "正在初始化数据库连接..." << endl;

    // 1. 获取 Connector/C++ 数据库连接（无 C API 相关代码）
    auto conn = getMySQLConnection();
    if (!conn) {
        return 1;
    }

    cout << "数据库连接成功！" << endl;

    // 2. 适配 9.6 版本：查询 MySQL 服务器版本（替代无效的 getServerVersion()）
    unique_ptr<Statement> version_stmt(conn->createStatement());
    unique_ptr<ResultSet> version_res(version_stmt->executeQuery("SELECT VERSION()"));
    if (version_res->next()) {
        cout << "MySQL 服务器版本：" << version_res->getString(1) << endl;
    }

    // 3. 测试添加职务
    addPosition(conn.get(), "经理");
    addPosition(conn.get(), "工程师");
    addPosition(conn.get(), "设计师");
    addPosition(conn.get(), "销售员");

    // 4. 打印职务列表
    vector<Position> positions = getAllPositions(conn.get());
    printPositions(positions);

    // 5. 测试添加人员
    addEmployee(conn.get(), "张三", 1);
    addEmployee(conn.get(), "李四", 2);
    addEmployee(conn.get(), "王五", 2);
    addEmployee(conn.get(), "赵六", 3);

    // 6. 打印所有人员
    vector<Employee> employees = getAllEmployees(conn.get());
    printEmployees(employees);

    // 7. 测试修改人员信息
    updateEmployee(conn.get(), 1, "张三(经理)", 1);

    // 8. 测试根据 ID 查询人员
    Employee emp = getEmployeeById(conn.get(), 1);
    cout << "\n查询到的人员信息：" << emp.name << " - " << emp.positionName << endl;

    // 9. 再次打印所有数据
    employees = getAllEmployees(conn.get());
    printPositions(positions);
    printEmployees(employees);

    // 10. 资源自动释放（智能指针，无需手动关闭，适配 Connector/C++）
    cout << "\n数据库连接已自动释放，程序运行结束。" << endl;

    return 0;
}