#include <iostream>
#include <string>
#include <vector>
// 仅保留 Connector/C++ 核心头文件，无任何 C API 痕迹
#include <mysql\jdbc.h>

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

// 补全：根据 ID 查询人员（Connector/C++ 实现，保持原有功能）
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
        pstmt->setInt(1, employeeId); // 绑定查询参数（人员ID）
        unique_ptr<ResultSet> res(pstmt->executeQuery()); // 执行查询

        // 如果查询到结果，赋值给 Employee 结构体
        if (res->next()) {
            emp.id = res->getInt("employee_id");
            emp.name = res->getString("employee_name");
            emp.positionId = res->getInt("position_id");
            emp.positionName = res->getString("position_name");
        } else {
            cout << "未查询到 ID 为 " << employeeId << " 的人员信息" << endl;
        }
    } catch (SQLException& e) {
        cerr << "根据 ID 查询人员失败：" << e.what() << endl;
    }

    return emp;
}

// 补充：删除人员（Connector/C++ 实现，完成「删」的功能）
bool deleteEmployee(Connection* conn, int employeeId) {
    if (!conn) {
        cerr << "无效的数据库连接！" << endl;
        return false;
    }

    try {
        string sql = "DELETE FROM employees WHERE employee_id = ?";
        unique_ptr<PreparedStatement> pstmt(conn->prepareStatement(sql));
        pstmt->setInt(1, employeeId); // 绑定删除参数（人员ID）
        int affectedRows = pstmt->executeUpdate(); // 执行删除，返回受影响的行数

        if (affectedRows > 0) {
            cout << "人员删除成功，ID：" << employeeId << endl;
            return true;
        } else {
            cout << "未找到 ID 为 " << employeeId << " 的人员，删除失败" << endl;
            return false;
        }
    } catch (SQLException& e) {
        cerr << "删除人员失败：" << e.what() << endl;
        return false;
    }
}

// 辅助函数：打印所有职务（方便测试）
void printAllPositions(Connection* conn) {
    vector<Position> positions = getAllPositions(conn);
    cout << "\n===== 所有职务列表 =====" << endl;
    for (const auto& pos : positions) {
        cout << "职务ID：" << pos.id << "，职务名称：" << pos.name << endl;
    }
}

// 辅助函数：打印所有人员（方便测试）
void printAllEmployees(Connection* conn) {
    vector<Employee> employees = getAllEmployees(conn);
    cout << "\n===== 所有人员列表 =====" << endl;
    for (const auto& emp : employees) {
        cout << "人员ID：" << emp.id << "，姓名：" << emp.name 
             << "，职务ID：" << emp.positionId << "，职务名称：" << emp.positionName << endl;
    }
}

// 主函数：测试所有增删改查功能
int main() {
    // 1. 获取数据库连接
    unique_ptr<Connection> conn = getMySQLConnection();
    if (!conn) {
        cerr << "数据库连接初始化失败，程序退出！" << endl;
        return 1;
    }
    cout << "数据库连接成功！" << endl;

    // 2. 测试：添加职务
    addPosition(conn.get(), "软件工程师");
    addPosition(conn.get(), "产品经理");
    printAllPositions(conn.get());

    // 3. 测试：添加人员（假设职务ID 1 是软件工程师，ID 2 是产品经理）
    addEmployee(conn.get(), "张三", 1);
    addEmployee(conn.get(), "李四", 2);
    printAllEmployees(conn.get());

    // 4. 测试：根据 ID 查询人员（查询 ID=1 的人员）
    Employee emp1 = getEmployeeById(conn.get(), 1);
    if (emp1.id != 0) {
        cout << "\n===== 单个人员查询结果 =====" << endl;
        cout << "人员ID：" << emp1.id << "，姓名：" << emp1.name 
             << "，职务ID：" << emp1.positionId << "，职务名称：" << emp1.positionName << endl;
    }

    // 5. 测试：更新人员信息（将 ID=1 的人员改为「张三丰」，职务ID=2）
    updateEmployee(conn.get(), 1, "张三丰", 2);
    printAllEmployees(conn.get());

    // 6. 测试：删除人员（删除 ID=2 的人员）
    deleteEmployee(conn.get(), 2);
    printAllEmployees(conn.get());

    return 0;
}