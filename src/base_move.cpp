#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>

ros::Publisher cmdVelPub;

void poseCb(const geometry_msgs::Pose2DConstPtr msg)
{
    float target_x_ = 0.0;
    float target_y_ = 0.0;
    target_x_ = msg->x;
    target_y_= msg->y;
    float dis = sqrt(pow(target_x_,2) + pow(target_y_,2));
    if (dis > 0.5)
    {
        geometry_msgs::Twist speed;
        speed.linear.x = target_x_ / 10;
        speed.linear.y = target_y_ / 10;
        if (fabs(speed.linear.x) > 0.01)
            speed.linear.x = speed.linear.x / fabs(speed.linear.x) * 0.01;
        if (fabs(speed.linear.y) > 0.01)
            speed.linear.y = speed.linear.y / fabs(speed.linear.y) * 0.01;
        cmdVelPub.publish(speed);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "base_move");
    std::string topic = "/cmd_vel";
    ros::NodeHandle node;

    cmdVelPub = node.advertise<geometry_msgs::Twist>(topic, 1);

    ros::Subscriber sub = node.subscribe<geometry_msgs::Pose2D>("grasp/target_pose", 1, poseCb);

    ROS_INFO("Base movement control start...");

    while (ros::ok())
    {
        ros::spinOnce();
    }

    return 0;
}
