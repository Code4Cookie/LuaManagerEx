#pragma once

class Entity
{
public:
	inline const char* GetName()
	{
		return m_szName;
	}

	inline const char& GetModel()
	{
		return *m_szModel;
	}

	inline void SetModel(const char* szModel)
	{
		m_szModel = szModel;
	}

	inline int GetHealth()
	{
		return m_iHealth;
	}

	inline void SetHealth(int iHealth)
	{
		m_iHealth = iHealth;
	}
protected:
	const char* m_szName;
	const char* m_szModel;
	int m_iHealth = 0;
};



class Player : public Entity
{
public:
	Player(const char* szNick) : Entity()
	{
		m_szNick = szNick;
	}

	inline const char* GetNick()
	{
		return m_szNick;
	}

	void Jump()
	{
		
	}

	void Shoot()
	{

	}

private:
	const char* m_szNick;
};

class NPC : public Entity
{
public:
	enum Agression { Peaceful, Threatening, Dangerous };

	NPC(Agression iAggression) : Entity()
	{
		m_iAggression = iAggression;
		m_iHealth = 200;
	}

	inline uint8_t GetAggression()
	{
		return m_iAggression;
	}

	inline void SetAggression(int iAggro)
	{
		Agression iRealAggro = static_cast<Agression>(iAggro);

		m_iAggression = iRealAggro;
	}

	inline void SetAggression(Agression iAggro)
	{
		m_iAggression = iAggro;
	}

private:
	Agression m_iAggression = Threatening;
};